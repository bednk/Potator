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
		_graph{ _transforms, _tree },
		_renderer{ _device.get(), _meshes, _transforms, _materials },
		_views{ _transforms, _cameras, _graph, _device.get(), settings.Width / (float)settings.Height },
		_movementSystem { _transforms, _movements },
		_stepTracker { 30 },
		_commandDispatcher { _commands },
		_loader { _device.get(), _shaderCache.get(), _graph, _views, _meshes, _transforms, _materials}
	{
		_stepTracker.Subscribe(&_movementSystem);

		Entity camera = _views.GetActive();
		std::shared_ptr<IInputHandler> controllerHandler = _inputHandlers.emplace_back(std::make_shared<ControllerMovementInputHandler>(_commandDispatcher, _movements, _transforms));
		controllerHandler->SetEntity(camera);
		_views.ViewChanged.connect([this, controllerHandler](Entity e) { controllerHandler->SetEntity(e); });

		WindowResized.connect([this](unsigned int w, unsigned int h) { _views.OnWindowResized(w, h); });
		WindowResized.connect([this](unsigned int w, unsigned int h) { _device->OnWindowResized(w, h); });
	}

	void Engine::Run()
	{
		while (_mainWindow.isOpen())
		{
			_stepTracker.OnFrameStart();
			while (const std::optional event = _mainWindow.pollEvent())
			{
				if (!event)
				{
					continue;
				}

				if (event->is<sf::Event::Closed>())
				{
					_mainWindow.close();
				}

				else if (event->is<sf::Event::Resized>())
				{
					const auto* resizeEvent = event->getIf<sf::Event::Resized>();
					WindowResized(resizeEvent->size.x, resizeEvent->size.y);
				}
			}

			_device->Clear(0, 0, 0, 1);
			for (size_t i = 0; i < _inputHandlers.size(); i++)
			{
				_inputHandlers[i]->Handle();
			}
			_commandDispatcher.Dispatch();
			_stepTracker.Update();
			_graph.UpdateTransforms();
			_views.UpdateView();
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
		return _graph;
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