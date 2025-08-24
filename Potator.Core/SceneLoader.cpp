#include "SceneLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <queue>
#include "EntityRegistry.h"
#include "TexturedVertex.h"
#include "VertexBuffer.h"
#include "HlslShader.h"
#include "RgbaTextureContainer.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <boost/dll/runtime_symbol_info.hpp>

namespace fs = std::filesystem;
using namespace Potator;

static Eigen::Matrix4f GetEigenMatrix(const aiMatrix4x4t<float>& matrix)
{
	Eigen::Matrix4f result;

	result << matrix.a1, matrix.a2, matrix.a3, matrix.a4,
		matrix.b1, matrix.b2, matrix.b3, matrix.b4,
		matrix.c1, matrix.c2, matrix.c3, matrix.c4,
		matrix.d1, matrix.d2, matrix.d3, matrix.d4;

	return result;
}

static RgbaTextureContainer GetTexture(aiTexture* tex)
{
	RgbaTextureContainer img = {};

	if (tex->mHeight > 0)
	{
		img.Width = tex->mWidth;
		img.Height = tex->mHeight;
		size_t nbytes = tex->mWidth * tex->mHeight * 4;
		img.Pixels.resize(nbytes);
		memcpy(img.Pixels.data(), tex->pcData, nbytes);
	}
	else
	{
		int width, height, _;
		unsigned char* decodedPixels = stbi_load_from_memory
		(
			reinterpret_cast<const stbi_uc*>(tex->pcData),
			tex->mWidth,
			&width,
			&height,
			&_,
			4
		);

		if (decodedPixels)
		{
			img.Width = width;
			img.Height = height;
			img.Pixels.assign(decodedPixels, decodedPixels + (width * height * 4));
			stbi_image_free(decodedPixels);
		}
		else
		{
			throw std::runtime_error("Texture decoding failed");
		}
	}
	return img;
}

static std::vector<ShaderResourceHandle> LoadTextures(const aiScene* scene, IGraphicsDevice* device)
{
	std::vector<ShaderResourceHandle> result;
	for (size_t i = 0; i < scene->mNumTextures; i++)
	{
		aiTexture* source = scene->mTextures[i];
		RgbaTextureContainer data = GetTexture(source);
		result.push_back(device->Create2dTexture(&data));
	}

	return result;
}

static std::vector<MaterialComponent> LoadMaterials(const aiScene* scene, IGraphicsDevice* device, IShaderCache* shaderCache)
{
	auto vsBinary = shaderCache->GetShaderBinary("vs_textured");
	static std::vector<MaterialComponent> result;
	std::vector<ShaderResourceHandle> textures = LoadTextures(scene, device);
	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];
		aiString texPath;
		if (material->GetTexture(aiTextureType_BASE_COLOR, 0, &texPath) != AI_SUCCESS)
		{
			material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);
		}
		int texIndex = std::atoi(texPath.C_Str() + 1);

		MaterialComponent& materialComponent = result.emplace_back();
		materialComponent.VertexShader = shaderCache->GetVertexShaderHandle("vs_textured");
		materialComponent.VsBinary = vsBinary;
		materialComponent.PixelShader = shaderCache->GetPixelShaderHandle("ps_textured");
		materialComponent.InputLayout = device->CreateInputLayout(TexturedVertex::GetLayout(), vsBinary.get());
		materialComponent.Texture = textures[texIndex];
	}
	return result;
}

static unsigned int GetUvChannelIndex(const aiScene* scene, aiMesh* mesh)
{
	aiString _;
	unsigned int uvIndex = 0;
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	if (!material->GetTexture(aiTextureType_BASE_COLOR, 0, &_, nullptr, &uvIndex) == AI_SUCCESS)
	{
		material->GetTexture(aiTextureType_DIFFUSE, 0, &_, nullptr, &uvIndex);
	}
	return uvIndex;
}

static std::vector<MeshComponent> LoadMeshes(const aiScene* scene, IGraphicsDevice* device)
{
	std::vector<MeshComponent> result;
	for (size_t i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];
		MeshComponent& meshComponent = result.emplace_back();
		meshComponent.StartIndexLocation = 0;
		std::vector<TexturedVertex> vertices;
		std::vector<unsigned short> indices;

		for (size_t f = 0; f < mesh->mNumFaces; f++)
		{
			aiFace face = mesh->mFaces[f];

			for (size_t i = 0; i < face.mNumIndices; i++)
			{
				auto indice = face.mIndices[i];
				indices.push_back(indice);
			}
		}

		unsigned int uvChannel = GetUvChannelIndex(scene, mesh);
		for (size_t v = 0; v < mesh->mNumVertices; v++)
		{
			aiVector3D pos = mesh->mVertices[v];
			aiVector3D uv = mesh->mTextureCoords[uvChannel][v];
	
			TexturedVertex& vertex = vertices.emplace_back();
			vertex.Position = { pos.x, pos.y, pos.z, 1.0f };
			vertex.Uv = { uv.x, uv.y };
		}

		VertexBuffer<TexturedVertex> cpuVBuf(vertices);
		IndexBuffer cpuIBuf(indices);

		meshComponent.IndexCount = (UINT)indices.size();
		meshComponent.VertexBuffer = device->Create(&cpuVBuf);
		meshComponent.IndexBuffer = device->Create(&cpuIBuf);
	}

	return result;
}

Potator::SceneLoader::SceneLoader(IGraphicsDevice* device,
		IShaderCache* shaderCache,
		SceneGraph& graph,
		ViewManager& views,
		ComponentStorage<MeshComponent>& meshes,
		ComponentStorage<TransformComponent>& transforms,
		ComponentStorage<MaterialComponent>& materials) :
	_device { device },
	_shaderCache { shaderCache },
	_graph { graph },
	_views { views },
	_meshes { meshes },
	_transforms { transforms },
	_materials { materials }
{
}

void Potator::SceneLoader::Load(fs::path path)
{
	if (!path.has_parent_path() || path.parent_path().empty())
	{
		path = boost::dll::program_location().parent_path().string() / fs::path("resources\\scenes") / path.string();
	}

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path.string(),
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_GenNormals |
		aiProcess_JoinIdenticalVertices);

	if (scene->mRootNode == nullptr)
	{
		return;
	}


	std::vector<MaterialComponent> materials = LoadMaterials(scene, _device, _shaderCache);
	std::vector<MeshComponent> meshComponents = LoadMeshes(scene, _device);

	std::queue<aiNode*> queue;
	std::map<aiNode*, Entity> parents;

	queue.push(scene->mRootNode);
	parents[scene->mRootNode] = NONE_ENTITY;

	while (!queue.empty())
	{
		aiNode* node = queue.front();
		queue.pop();
		Entity nodeEntity = EntityRegistry::Instance().GetNew();
		TransformComponent nodeTransform;
		nodeTransform.Local = GetEigenMatrix(node->mTransformation);
		_graph.AddNode(nodeEntity, nodeTransform, parents[node]);

		for (size_t i = 0; i < node->mNumMeshes; i++)
		{
			unsigned int meshIdx = node->mMeshes[i];
			unsigned int materialIdx = scene->mMeshes[node->mMeshes[i]]->mMaterialIndex;
			MeshComponent meshComponent = meshComponents[meshIdx]; //intentional copy
			MaterialComponent materialComponent = materials[materialIdx];

			Entity meshEntity = EntityRegistry::Instance().GetNew();
			TransformComponent meshTransform;
			_graph.AddNode(meshEntity, meshTransform, nodeEntity);
			_meshes.Store(meshEntity, meshComponent);
			_materials.Store(meshEntity, materialComponent);
		}

		for (size_t i = 0; i < node->mNumChildren; i++)
		{
			queue.push(node->mChildren[i]);
			parents[node->mChildren[i]] = nodeEntity;
		}
	}
}
