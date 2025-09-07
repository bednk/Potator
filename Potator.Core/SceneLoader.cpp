#include "SceneLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <queue>
#include "EntityRegistry.h"
#include "CompositeVertex.h"
#include "VertexBuffer.h"
#include "HlslShader.h"
#include "RgbaTextureContainer.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <boost/dll/runtime_symbol_info.hpp>
#include <nlohmann/json.hpp>
#include <fstream>


namespace fs = std::filesystem;
using namespace Potator;

#pragma region static_helpers
struct GLBHeader
{
	uint32_t magic;
	uint32_t version;
	uint32_t length;
};

struct GLBChunkHeader
{
	uint32_t chunkLength;
	uint32_t chunkType;
};

static nlohmann::json LoadGLBJson(const std::string& path)
{
	static const int glbMagic = 0x46546C67;
	static const int jsonChunk = 0x4E4F534A;

	std::ifstream file(path, std::ios::binary);
	GLBHeader header = {};
	file.read(reinterpret_cast<char*>(&header), sizeof(header));
	if (header.magic != glbMagic)
	{
		throw std::runtime_error("Not a GLB file");
	}

	GLBChunkHeader chunkHeader = {};
	file.read(reinterpret_cast<char*>(&chunkHeader), sizeof(chunkHeader));
	if (chunkHeader.chunkType != jsonChunk)
	{
		throw std::runtime_error("First chunk is not JSON");
	}

	std::vector<char> jsonData(chunkHeader.chunkLength);
	file.read(jsonData.data(), chunkHeader.chunkLength);

	return nlohmann::json::parse(jsonData.begin(), jsonData.end());
}

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
		size_t nbytes = static_cast<size_t>(tex->mWidth) * tex->mHeight * 4;
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

static std::unordered_map<std::string, std::string> GetLuaScripts(fs::path path)
{
	std::unordered_map<std::string, std::string> result;

	nlohmann::json js = LoadGLBJson(path.string());
	for (size_t i = 0; i < js["nodes"].size(); i++)
	{
		auto& node = js["nodes"][i];
		if (node.contains("extras"))
		{
			auto& extras = node["extras"];
			if (extras.contains("lua"))
			{
				auto& script = extras["lua"];
				result[node["name"]] = script;
			}
		}
	}

	return result;
}

static std::unordered_map<std::string, std::string> GetCustomPixelShaderNames(fs::path path)
{
	std::unordered_map<std::string, std::string> result;

	nlohmann::json js = LoadGLBJson(path.string());
	for (size_t i = 0; i < js["nodes"].size(); i++)
	{
		auto& node = js["nodes"][i];
		if (node.contains("extras"))
		{
			auto& extras = node["extras"];
			if (extras.contains("ps"))
			{
				auto& script = extras["ps"];
				result[node["name"]] = script;
			}
		}
	}

	return result;
}

static std::unordered_map<std::string, PointLightComponent> GetLights(aiLight** lights, unsigned int count)
{
	std::unordered_map<std::string, PointLightComponent> result;
	for (size_t i = 0; i < count; i++)
	{
		aiLight* light = lights[i];
		std::string name = { light->mName.C_Str() };
		result[name] = PointLightComponent{};
		result[name].Color = { light->mColorDiffuse.r, light->mColorDiffuse.g, light->mColorDiffuse.b, 0.0f };
		result[name].Color.normalize(); // throw out intensity (for now (hue hue hue))
	}

	return result;
}
#pragma endregion


Potator::SceneLoader::SceneLoader(IGraphicsDevice& device,
		IShaderCache& shaderCache,
		SceneGraph& graph,
		ViewManager& views,
		Components& components) :
	_device { device },
	_shaderCache { shaderCache },
	_sceneGraph { graph },
	_views { views },
	_components{ components }
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

	std::vector<MaterialComponent> materials = LoadMaterials(scene);
	std::vector<MeshComponent> meshComponents = LoadMeshes(scene);
	std::unordered_map<std::string, PointLightComponent> lights = GetLights(scene->mLights, scene->mNumLights);
	std::unordered_map<std::string, std::string> scripts = GetLuaScripts(path);
	std::unordered_map<std::string, std::string> shaders = GetCustomPixelShaderNames(path);

	std::queue<aiNode*> queue;
	std::map<aiNode*, Entity> parents;

	queue.push(scene->mRootNode);
	parents[scene->mRootNode] = NONE_ENTITY;

	while (!queue.empty())
	{
		aiNode* node = queue.front();
		std::string name = { node->mName.C_Str() };
		queue.pop();
		Entity nodeEntity = EntityRegistry::Instance().GetNew();
		TransformComponent nodeTransform;
		VelocityComponent nodeVelocity;
		nodeTransform.Local = GetEigenMatrix(node->mTransformation);
		_sceneGraph.AddNode(nodeEntity, nodeTransform, parents[node]);
		_components.Movements.Store(nodeEntity, nodeVelocity);

		for (size_t i = 0; i < node->mNumMeshes; i++)
		{
			unsigned int meshIdx = node->mMeshes[i];
			unsigned int materialIdx = scene->mMeshes[node->mMeshes[i]]->mMaterialIndex;
			MeshComponent meshComponent = meshComponents[meshIdx]; //intentional copy
			MaterialComponent materialComponent = materials[materialIdx];

			if (shaders.contains(name))
			{
				materialComponent.PixelShader = _shaderCache.GetPixelShaderHandle(shaders[name]);
			}

			Entity meshEntity = EntityRegistry::Instance().GetNew();
			TransformComponent meshTransform;
			_sceneGraph.AddNode(meshEntity, meshTransform, nodeEntity);
			_components.Meshes.Store(meshEntity, meshComponent);
			_components.Materials.Store(meshEntity, materialComponent);
		}

		if (scripts.contains(name))
		{
			ScriptComponent component = {};
			component.Script = scripts[name];
			_components.Scripts.Store(nodeEntity, component);
		}

		if (lights.contains(name))
		{
			_components.PointLights.Store(nodeEntity, lights[name]);
		}

		for (size_t i = 0; i < node->mNumChildren; i++)
		{
			queue.push(node->mChildren[i]);
			parents[node->mChildren[i]] = nodeEntity;
		}
	}
}

std::vector<ShaderResourceHandle> Potator::SceneLoader::LoadTextures(const aiScene* scene)
{
	std::vector<ShaderResourceHandle> result;
	for (size_t i = 0; i < scene->mNumTextures; i++)
	{
		aiTexture* source = scene->mTextures[i];
		RgbaTextureContainer data = GetTexture(source);
		result.push_back(_device.Create2dTexture(&data));
	}

	return result;
}

std::vector<MaterialComponent> Potator::SceneLoader::LoadMaterials(const aiScene* scene)
{
	auto vsBinary = _shaderCache.GetShaderBinary("vs_composite");
	static std::vector<MaterialComponent> result;
	std::vector<ShaderResourceHandle> textures = LoadTextures(scene);
	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		MaterialComponent& materialComponent = result.emplace_back();
		MaterialDescriptor descriptor = {};
		aiMaterial* material = scene->mMaterials[i];

		aiColor4D diffuse;
		if (aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse) == AI_SUCCESS
			|| aiGetMaterialColor(material, AI_MATKEY_BASE_COLOR, &diffuse) == AI_SUCCESS)
		{
			descriptor.HasColor = true;
			descriptor.Color = { diffuse.r, diffuse.g, diffuse.b, diffuse.a };
		}

		aiString texPath;
		if (material->GetTexture(aiTextureType_BASE_COLOR, 0, &texPath) == AI_SUCCESS
			|| material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS)
		{
			descriptor.HasTexture = true;
			int texIndex = std::atoi(texPath.C_Str() + 1);
			materialComponent.Texture = textures[texIndex];
		}

		ConstantBuffer<MaterialDescriptor> descriptorBuffer(descriptor);

		materialComponent.DescriptorHandle = _device.Create(&descriptorBuffer);
		materialComponent.VertexShader = _shaderCache.GetVertexShaderHandle("vs_composite");
		materialComponent.VsBinary = vsBinary;
		materialComponent.PixelShader = _shaderCache.GetPixelShaderHandle("ps_composite");
		materialComponent.InputLayout = _device.CreateInputLayout(CompositeVertex::GetLayout(), vsBinary.get());

	}
	return result;
}

std::vector<MeshComponent> Potator::SceneLoader::LoadMeshes(const aiScene* scene)
{
	std::vector<MeshComponent> result;
	for (size_t i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];
		bool isColored = mesh->HasVertexColors(0);
		bool hasNormals = mesh->HasNormals();
		MeshComponent& meshComponent = result.emplace_back();
		meshComponent.StartIndexLocation = 0;
		std::vector<CompositeVertex> vertices;
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
			CompositeVertex& vertex = vertices.emplace_back();

			aiVector3D pos = mesh->mVertices[v];
			vertex.Position = { pos.x, pos.y, pos.z, 1.0f };

			aiVector3D uv = mesh->mTextureCoords[uvChannel][v];
			vertex.Uv = { uv.x, uv.y };

			if (isColored)
			{
				aiColor4D vertexColor = mesh->mColors[0][v];
				vertex.Color = { vertexColor.r, vertexColor.g, vertexColor.b, vertexColor.a };
			}

			if (hasNormals)
			{
				aiVector3D normal = mesh->mNormals[v];
				vertex.Normal = { normal.x, normal.y, normal.z };
			}
		}

		VertexBuffer<CompositeVertex> cpuVBuf(vertices);
		IndexBuffer cpuIBuf(indices);

		meshComponent.IndexCount = (UINT)indices.size();
		meshComponent.VertexBuffer = _device.Create(&cpuVBuf);
		meshComponent.IndexBuffer = _device.Create(&cpuIBuf);
	}

	return result;
}
