#pragma once
#include "Components.h"
#include "IGraphicsDevice.h"
#include "IShaderCache.h"
#include "SceneGraph.h"
#include "ViewManager.h"
#include <filesystem>
#include <assimp/scene.h>

namespace Potator
{
	class SceneLoader
	{
	public:
		SceneLoader(IGraphicsDevice& device,
			IShaderCache& shaderCache,
			SceneGraph& graph,
			ViewManager& views,
			Components& components);
		void Load(std::filesystem::path path);
	private:
		std::vector<ShaderResourceHandle> LoadTextures(const aiScene* scene);
		std::vector<MaterialComponent> LoadMaterials(const aiScene* scene);
		std::vector<MeshComponent> LoadMeshes(const aiScene* scene);
		IGraphicsDevice& _device;
		IShaderCache& _shaderCache;
		SceneGraph& _sceneGraph;
		ViewManager& _views;
		Components& _components;
	};
}