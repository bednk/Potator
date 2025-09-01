#include "pch.h"
#include "Engine.h"
#include "DeviceFactory.h"


namespace Potator
{
	Engine::Engine() : Engine(LaunchingParams{})
	{

	}

	Engine::Engine(LaunchingParams settings) :
		_mainWindow{ sf::VideoMode({ settings.Width, settings.Height }), settings.Title },
		_device{ DeviceFactory::GetDevice(settings, _mainWindow.getNativeHandle()) },
		_shaderCache { DeviceFactory::GetShaderCache(settings.Api, _device.get() )},
		_sceneGraph{ _transforms, _nodes },
		_renderer{ _device.get(), _meshes, _transforms, _materials },
		_views{ _transforms, _cameras, _sceneGraph, _device.get(), settings.Width / (float)settings.Height },
		_movementSystem { _transforms, _movements },
		_fixedStepTracker { 30 },
		_commandDispatcher { _commands },
		_loader { _device.get(), _shaderCache.get(), _sceneGraph, _views, _meshes, _transforms, _materials, _movements, _scripts, _lights },
		_lighting { _lights, _transforms, _device.get() },
		_windowHandler { &_mainWindow },
		_scripting { _commandDispatcher, _scripts, _movements, _transforms }
	{
		_fixedStepTracker.Subscribe(&_movementSystem);

		Entity camera = _views.GetActive();
		std::shared_ptr<IInputHandler> controllerHandler = std::make_shared<ControllerMovementInputHandler>(_commandDispatcher, _movements, _transforms);
		controllerHandler->SetEntity(camera);
		_views.ViewChanged.connect([this, controllerHandler](Entity e) { controllerHandler->SetEntity(e); });

		_windowHandler.RegisterInputHandler(controllerHandler);
		_windowHandler.WindowResized.connect([this](unsigned int w, unsigned int h) { _views.OnWindowResized(w, h); });
		_windowHandler.WindowResized.connect([this](unsigned int w, unsigned int h) { _device->OnWindowResized(w, h); });
	}

	void Engine::Run()
	{
		while (_mainWindow.isOpen())
		{
			_fixedStepTracker.MarkFrameStart();

			_windowHandler.Handle();
			_commandDispatcher.Dispatch();

			_scripting.Update();
			_fixedStepTracker.Update();
			_sceneGraph.Update();
			_lighting.Update();
			_views.Update();

			_device->Clear(0, 0, 0, 1);
			_renderer.Render();
			_device->Present();
		}
	}


	ComponentStorage<MeshComponent>& Engine::GetMeshes()
	{
		return _meshes;
	}

	ComponentStorage<TransformComponent>& Engine::GetTransforms()
	{
		return _transforms;
	}

	ComponentStorage<MovementComponent>& Engine::GetMovements()
	{
		return _movements;
	}

	ComponentStorage<MaterialComponent>& Engine::GetMaterials()
	{
		return _materials;
	}

	SceneGraph& Engine::GetSceneGraph()
	{
		return _sceneGraph;
	}

	ViewManager& Engine::GetViewManager()
	{
		return _views;
	}

	CommandDispatcher& Engine::GetCommandDispatcher()
	{
		return _commandDispatcher;
	}

	MovementSystem& Engine::GetMovementSystem()
	{
		return _movementSystem;
	}

	IGraphicsDevice* Engine::GetGraphicsDevice()
	{
		return _device.get();
	}

	SceneLoader& Engine::GetLoader()
	{
		return _loader;
	}

	Engine::~Engine() = default;
}