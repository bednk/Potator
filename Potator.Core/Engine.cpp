#include "pch.h"
#include "Engine.h"
#include "DeviceFactory.h"


namespace Potator
{
	Engine::Engine() : Engine(GpuApi::Dx11)
	{

	}

	Engine::Engine(GpuApi api) :
		_mainWindow{ sf::VideoMode({ 800, 600 }), "Potator Engine" },
		_device{ DeviceFactory::GetDevice(api, _mainWindow.getNativeHandle()) },
		_graph{ _transforms, _tree },
		_renderer{ _device.get(), _meshes, _transforms},
		_views{ _transforms, _graph, _device.get() },
		_movementSystem { _transforms, _movements },
		_stepTracker { 30 },
		_commandDispatcher { _commands },
		_cameraHandler { _commandDispatcher, _movementSystem }
	{
		_stepTracker.Subscribe(&_movementSystem);
		Entity camera = _registry.GetNew();
		_views.Add(camera);
		_cameraHandler.SetEntity(camera);
		_views.ViewChanged.connect([this](Entity e) { _cameraHandler.SetEntity(e); });
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
			}

			_device->Clear(0, 0, 0, 1);
			_cameraHandler.Handle();
			_commandDispatcher.Dispatch();
			_stepTracker.Update();
			_graph.UpdateTransforms();
			_views.UpdateView();
			_renderer.Render();
			_device->Present();
		}
	}

	EntityRegistry& Engine::GetEntityRegistry()
	{
		return _registry;
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

	Engine::~Engine() = default;
}