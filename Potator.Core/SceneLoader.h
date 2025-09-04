#pragma once
#include <string>
#include <filesystem>
#include "SceneGraph.h"
#include "ViewManager.h"
#include "IShaderCache.h"
#include "ComponentStorage.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"
#include "MovementComponent.h"
#include "ScriptComponent.h"
#include "PointLightComponent.h"

namespace Potator
{
	class SceneLoader
	{
	public:
		SceneLoader(std::shared_ptr<IGraphicsDevice> device,
			std::shared_ptr<IShaderCache> shaderCache,
			SceneGraph& graph,
			ViewManager& views,
			ComponentStorage<MeshComponent>& meshes,
			ComponentStorage<TransformComponent>& transforms,
			ComponentStorage<MaterialComponent>& materials,
			ComponentStorage<MovementComponent>& movements,
			ComponentStorage<ScriptComponent>& scripts,
			ComponentStorage<PointLightComponent>& lights);
		void Load(std::filesystem::path path);
	private:
		std::shared_ptr<IGraphicsDevice> _device;
		std::shared_ptr<IShaderCache> _shaderCache;
		SceneGraph& _sceneGraph;
		ViewManager& _views;
		ComponentStorage<MeshComponent>& _meshes;
		ComponentStorage<TransformComponent>& _transforms;
		ComponentStorage<MaterialComponent>& _materials;
		ComponentStorage<MovementComponent>& _movements;
		ComponentStorage<ScriptComponent>& _scripts;
		ComponentStorage<PointLightComponent>& _lights;
	};
}