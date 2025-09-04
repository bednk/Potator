#include "SceneLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
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
	GLBHeader header;
	file.read(reinterpret_cast<char*>(&header), sizeof(header));
	if (header.magic != glbMagic)
	{
		throw std::runtime_error("Not a GLB file");
	}

	GLBChunkHeader chunkHeader;
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
	auto vsBinary = shaderCache->GetShaderBinary("vs_composite");
	static std::vector<MaterialComponent> result;
	std::vector<ShaderResourceHandle> textures = LoadTextures(scene, device);
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

		materialComponent.DescriptorHandle = device->Create(&descriptorBuffer);
		materialComponent.VertexShader = shaderCache->GetVertexShaderHandle("vs_composite");
		materialComponent.VsBinary = vsBinary;
		materialComponent.PixelShader = shaderCache->GetPixelShaderHandle("ps_composite");
		materialComponent.InputLayout = device->CreateInputLayout(CompositeVertex::GetLayout(), vsBinary.get());
		
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
		meshComponent.VertexBuffer = device->Create(&cpuVBuf);
		meshComponent.IndexBuffer = device->Create(&cpuIBuf);
	}

	return result;
}

static std::string ToString(const aiString& source)
{
	return std::string{ source.C_Str() };
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

Potator::SceneLoader::SceneLoader(std::shared_ptr<IGraphicsDevice> device,
		std::shared_ptr<IShaderCache> shaderCache,
		SceneGraph& graph,
		ViewManager& views,
		ComponentStorage<MeshComponent>& meshes,
		ComponentStorage<TransformComponent>& transforms,
		ComponentStorage<MaterialComponent>& materials,
		ComponentStorage<MovementComponent>& movements,
		ComponentStorage<ScriptComponent>& scripts,
	    ComponentStorage<PointLightComponent>& lights) :
	_device { device },
	_shaderCache { shaderCache },
	_sceneGraph { graph },
	_views { views },
	_meshes { meshes },
	_transforms { transforms },
	_materials { materials },
	_movements { movements },
	_scripts { scripts },
	_lights { lights }
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

	std::vector<MaterialComponent> materials = LoadMaterials(scene, _device.get(), _shaderCache.get());
	std::vector<MeshComponent> meshComponents = LoadMeshes(scene, _device.get());
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
		std::string name = ToString(node->mName);
		queue.pop();
		Entity nodeEntity = EntityRegistry::Instance().GetNew();
		TransformComponent nodeTransform;
		MovementComponent nodeMovement;
		nodeTransform.Local = GetEigenMatrix(node->mTransformation);
		_sceneGraph.AddNode(nodeEntity, nodeTransform, parents[node]);
		_movements.Store(nodeEntity, nodeMovement);

		for (size_t i = 0; i < node->mNumMeshes; i++)
		{
			unsigned int meshIdx = node->mMeshes[i];
			unsigned int materialIdx = scene->mMeshes[node->mMeshes[i]]->mMaterialIndex;
			MeshComponent meshComponent = meshComponents[meshIdx]; //intentional copy
			MaterialComponent materialComponent = materials[materialIdx];

			if (shaders.contains(name))
			{
				materialComponent.PixelShader = _shaderCache->GetPixelShaderHandle(shaders[name]);
			}

			Entity meshEntity = EntityRegistry::Instance().GetNew();
			TransformComponent meshTransform;
			_sceneGraph.AddNode(meshEntity, meshTransform, nodeEntity);
			_meshes.Store(meshEntity, meshComponent);
			_materials.Store(meshEntity, materialComponent);
		}

		if (scripts.contains(name))
		{
			ScriptComponent component = {};
			component.Script = scripts[name];
			_scripts.Store(nodeEntity, component);
		}

		if (lights.contains(name))
		{
			_lights.Store(nodeEntity, lights[name]);
		}

		for (size_t i = 0; i < node->mNumChildren; i++)
		{
			queue.push(node->mChildren[i]);
			parents[node->mChildren[i]] = nodeEntity;
		}
	}
}
