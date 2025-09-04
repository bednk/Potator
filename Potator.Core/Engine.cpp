#include "pch.h"
#include "Engine.h"


namespace Potator
{
	Engine::Engine(WindowWrapper& mainWindow,
			std::shared_ptr<IGraphicsDevice> device,
			std::shared_ptr<IShaderCache> shaderCache,
			Systems& systems,
			ComponentStorage<MovementComponent>& movements,
			ComponentStorage<TransformComponent>& transforms) :
		_mainWindow{ mainWindow.Window },
		_device{ device },
		_shaderCache{ shaderCache },
		_systems{ systems },
		_movements{ movements },
		_transforms{ transforms }
	{
		_systems.FixedStepTracker.Subscribe(&_systems.MovementSystem);

		Entity camera = _systems.Views.GetActive();
		std::shared_ptr<IInputHandler> controllerHandler = std::make_shared<ControllerMovementInputHandler>(_systems.CommandDispatcher, _movements, _transforms);
		controllerHandler->SetEntity(camera);
		_systems.Views.ViewChanged.connect([this, controllerHandler](Entity e) { controllerHandler->SetEntity(e); });

		_systems.WindowHandler.RegisterInputHandler(controllerHandler);
		_systems.WindowHandler.WindowResized.connect([this](unsigned int w, unsigned int h) { _systems.Views.OnWindowResized(w, h); });
		_systems.WindowHandler.WindowResized.connect([this](unsigned int w, unsigned int h) { _device->OnWindowResized(w, h); });
	}

	void Engine::Run()
	{
		while (_mainWindow.isOpen())
		{
			_systems.FixedStepTracker.MarkFrameStart();

			_systems.WindowHandler.Handle();
			_systems.CommandDispatcher.Dispatch();

			_systems.Scripting.Update();
			_systems.FixedStepTracker.Update();
			_systems.SceneGraph.Update();
			_systems.Lighting.Update();
			_systems.Views.Update();

			_device->Clear(0, 0, 0, 1);
			_systems.Renderer.Render();
			_device->Present();
		}
	}

	SceneLoader& Engine::GetLoader()
	{
		return _systems.Loader;
	}

	Engine::~Engine() = default;
}