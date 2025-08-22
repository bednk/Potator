#pragma once
#include <string>
#include <filesystem>
#include "SceneGraph.h"
#include "ViewManager.h"
#include "ComponentStorage.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"

namespace Potator
{
	class SceneLoader
	{
	public:
		SceneLoader(IGraphicsDevice* device, SceneGraph& graph, ViewManager& views, ComponentStorage<MeshComponent>& meshes, ComponentStorage<TransformComponent>& transforms, ComponentStorage<MaterialComponent>& materials);
		void Load(std::filesystem::path path);
	private:
		IGraphicsDevice* _device;
		SceneGraph& _graph;
		ViewManager& _views;
		ComponentStorage<MeshComponent>& _meshes;
		ComponentStorage<TransformComponent>& _transforms;
		ComponentStorage<MaterialComponent>& _materials;
	};
}