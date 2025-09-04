
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
#include "ControllerMovementInputHandler.h"
#include "Lighting.h"
#include "WindowHandler.h"
#include "ScriptingSystem.h"
#include "WindowWrapper.h"
#include "Systems.h"


namespace Potator
{
	class Engine
	{
	public:
		Engine(WindowWrapper& mainWindow,
			std::shared_ptr<IGraphicsDevice> device,
			std::shared_ptr<IShaderCache> shaderCache,
			Systems& systems,
			ComponentStorage<MovementComponent>& movements,
			ComponentStorage<TransformComponent>& transforms);
		SceneLoader& GetLoader();

		void Run();
		~Engine();
	private:
		sf::Window& _mainWindow;
		std::shared_ptr<IGraphicsDevice> _device;
		std::shared_ptr<IShaderCache> _shaderCache;
		Systems& _systems;
		ComponentStorage<MovementComponent>& _movements;
		ComponentStorage<TransformComponent>& _transforms;
	};
}
