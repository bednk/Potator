#pragma once
#include <memory>
#include "ComponentStorage.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "SceneNodeComponent.h"
#include "EntityRegistry.h"
#include "IGraphicsDevice.h"
#include "SceneGraph.h"
#include "ViewManager.h"
#include <SFML/Window.hpp>
#include "MeshRenderer.h"

namespace Potator
{
	class Engine
	{
	public:
		Engine();
		Engine(GpuApi api);
		EntityRegistry& GetEntityRegistry();
		ComponentStorage<MeshComponent>& GetMeshes();
		ComponentStorage<TransformComponent>& GetTransforms();
		SceneGraph& GetSceneGraph();
		ViewManager& GetViewManager();
		IGraphicsDevice* GetGraphicsDevice();

		void Run();
		~Engine();
	private:
		sf::Window _mainWindow;
		std::unique_ptr<IGraphicsDevice> _device;
		ComponentStorage<MeshComponent> _meshes;
		ComponentStorage<TransformComponent> _transforms;
		ComponentStorage<SceneNodeComponent> _tree;
		EntityRegistry _registry;
		MeshRenderer _renderer;
		SceneGraph _graph;
		ViewManager _views;
	};
}
