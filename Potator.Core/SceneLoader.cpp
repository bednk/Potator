#include "SceneLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <queue>
#include "EntityRegistry.h"
#include "ColoredVertex.h"
#include "VertexBuffer.h"
#include "HlslShader.h"

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

static std::vector<MeshComponent> LoadMeshes(aiMesh** meshes, unsigned int count, IGraphicsDevice* device)
{
	std::vector<MeshComponent> result;
	for (size_t i = 0; i < count; i++)
	{
		aiMesh* mesh = meshes[i];
		MeshComponent& meshComponent = result.emplace_back();
		meshComponent.StartIndexLocation = 0;
		std::vector<ColoredVertex> vertices;
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

		for (size_t v = 0; v < mesh->mNumVertices; v++)
		{
			aiVector3t<float> vec = mesh->mVertices[v];
			ColoredVertex& vertex = vertices.emplace_back();
			vertex.Position = { vec.x, vec.y, vec.z, 1.0f };
			vertex.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		}

		VertexBuffer<ColoredVertex> cpuVBuf(vertices);
		IndexBuffer cpuIBuf(indices);

		meshComponent.IndexCount = indices.size();
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
	auto vsBinary = _shaderCache->GetShaderBinary("vs_colored");
	MaterialComponent material =
	{
		_shaderCache->GetVertexShaderHandle("vs_colored"),
		vsBinary,
		_shaderCache->GetPixelShaderHandle("ps_colored"),
		_device->CreateInputLayout(ColoredVertex::GetLayout(), vsBinary.get())
	};

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

	std::vector<MeshComponent> meshComponents = LoadMeshes(scene->mMeshes, scene->mNumMeshes, _device);

	std::queue<aiNode*> queue;
	queue.push(scene->mRootNode);
	while (!queue.empty())
	{
		aiNode* node = queue.front();
		queue.pop();
		Entity nodeEntity = EntityRegistry::Instance().GetNew();
		TransformComponent nodeTransform;
		nodeTransform.Local = GetEigenMatrix(node->mTransformation);
		_graph.AddNode(nodeEntity, nodeTransform);

		for (size_t m = 0; m < node->mNumMeshes; m++)
		{
			unsigned int meshIdx = node->mMeshes[m];
			MeshComponent meshComponent = meshComponents[meshIdx]; //inentional copy

			Entity meshEntity = EntityRegistry::Instance().GetNew();
			TransformComponent meshTransform;
			_graph.AddNode(meshEntity, meshTransform, nodeEntity);
			_meshes.Store(meshEntity, meshComponent);
			_materials.Store(meshEntity, material);
		}
	}
}
