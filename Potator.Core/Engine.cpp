#include "Engine.h"
#include "ControllerMovementInputHandler.h"

namespace Potator
{
	Engine::Engine(WindowWrapper& mainWindow,
			std::shared_ptr<IGraphicsDevice> device,
			std::shared_ptr<IShaderCache> shaderCache,
			Systems& systems) :
		_mainWindow{ mainWindow },
		_device{ device },
		_shaderCache{ shaderCache },
		_systems{ systems }
	{
		_systems.FixedStepTracker.Subscribe(&_systems.MovementSystem);
		_systems.WindowHandler.WindowResized.connect([this](unsigned int w, unsigned int h) { _systems.Views.OnWindowResized(w, h); });
		_systems.WindowHandler.WindowResized.connect([this](unsigned int w, unsigned int h) { _device->OnWindowResized(w, h); });
	}

	void Engine::SetExtension(IEngineExtension* extension)
	{
		_extension = extension;
	}

	void Engine::Run()
	{
		bool executeExt = _extension.has_value();
		if (executeExt) _extension.value()->Initialize();

		while (!glfwWindowShouldClose(_mainWindow.Window))
		{
			_systems.FixedStepTracker.MarkFrameStart();

			if (executeExt) _extension.value()->OnFrameStarted();

			_systems.ImGui.NewFrame();
			if (!_systems.WindowHandler.Handle())
			{
				break;
			}

			_systems.CommandDispatcher.Dispatch();

			if (executeExt) _extension.value()->OnBeforeStateUpdated();

			_systems.Scripting.Update();
			_systems.FixedStepTracker.Update();
			_systems.SceneGraph.Update();
			_systems.Lighting.Update();
			_systems.Views.Update();

			if (executeExt) _extension.value()->OnBeforeSceneRendered();

			_systems.ImGui.Update();
			_device->Clear(0.0f, 0.0f, 0.0f, 1.0f);
			_systems.Renderer.Render();
			_systems.ImGui.Render();
			_device->Present();
		}

		if (executeExt) _extension.value()->Cleanup();
	}

	Engine::~Engine() = default;
}