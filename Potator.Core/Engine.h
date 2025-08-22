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
#include "MovementComponent.h"
#include "MovementSystem.h"
#include "FixedStepTracker.h"
#include "CommandDispatcher.h"
#include "MovementInputHandler.h"
#include "MaterialComponent.h"
#include "CameraComponent.h"
#include "LaunchingParams.h"
#include "SceneLoader.h"
#include <boost/signals2.hpp>


namespace Potator
{
	class Engine
	{
	public:
		Engine();
		Engine(LaunchingParams windowSettings);
		ComponentStorage<MeshComponent>& GetMeshes();
		ComponentStorage<TransformComponent>& GetTransforms();
		ComponentStorage<MovementComponent>& GetMovements();
		ComponentStorage<MaterialComponent>& GetMaterials();
		SceneGraph& GetSceneGraph();
		ViewManager& GetViewManager();
		CommandDispatcher& GetCommandDispatcher();
		MovementSystem& GetMovementSystem();
		IGraphicsDevice* GetGraphicsDevice();
		SceneLoader& GetLoader();

		void Run();
		~Engine();
	private:
		sf::Window _mainWindow;
		std::unique_ptr<IGraphicsDevice> _device;
		ComponentStorage<MeshComponent> _meshes;
		ComponentStorage<MaterialComponent> _materials;
		ComponentStorage<TransformComponent> _transforms;
		ComponentStorage<MovementComponent> _movements;
		ComponentStorage<SceneNodeComponent> _tree;
		ComponentStorage<CommandQueueComponent> _commands;
		ComponentStorage<CameraComponent> _cameras;
		MeshRenderer _renderer;
		SceneGraph _graph;
		ViewManager _views;
		MovementSystem _movementSystem;
		FixedStepTracker _stepTracker;
		CommandDispatcher _commandDispatcher;
		MovementInputHandler _cameraHandler;
		SceneLoader _loader;
		boost::signals2::signal<void(unsigned int, unsigned int)> WindowResized;
	};
}
