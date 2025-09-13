#include "Engine.h"
#include "imgui.h"
#include "EntityRegistry.h"
#include <boost/circular_buffer.hpp>
#include <cmath>
#include <format>

namespace Potator
{
	Engine::Engine(IGraphicsDevice& device, Systems& systems, Components& components) :
		_device{ device },
		_systems{ systems },
		_components{ components }
	{
		_systems.FixedStepTracker.Subscribe(&_systems.MovementSystem);
		_systems.WindowHandler.WindowResized.connect([this](unsigned int w, unsigned int h) { _systems.Views.OnWindowResized(w, h); });
		_systems.WindowHandler.WindowResized.connect([this](unsigned int w, unsigned int h) { _device.OnWindowResized(w, h); });
	}

	void Engine::SetExtension(IEngineExtension* extension)
	{
		_extension = extension;
	}

	void Engine::Run()
	{
#ifdef _DEBUG
		SetDebugWindow();
#endif
		bool executeExt = _extension.has_value();
		if (executeExt) _extension.value()->Initialize();

		while (_systems.WindowHandler.Handle())
		{
			_systems.Clock.Update();

			if (executeExt) _extension.value()->OnFrameStarted();

			_systems.ImGui.NewFrame();

			_systems.CommandDispatcher.Dispatch();

			if (executeExt) _extension.value()->OnBeforeStateUpdated();

			_systems.Scripting.Update();
			_systems.FixedStepTracker.Update();
			_systems.SceneGraph.Update();
			_systems.Lighting.Update();
			_systems.Views.Update();

			if (executeExt) _extension.value()->OnBeforeSceneRendered();

			_systems.ImGui.Update();
			_device.Clear(0.0f, 0.0f, 0.0f, 1.0f);
			_systems.Renderer.Render();
			_systems.ImGui.Render();
			_device.Present();
		}

		if (executeExt) _extension.value()->Cleanup();
	}

	Engine::~Engine() = default;

	void Engine::SetDebugWindow()
	{
		ImGuiComponent window;
		auto& clock = _systems.Clock;
		window.Draw = [&clock]()
			{
				static boost::circular_buffer<float> buffer(1000, 0.0f);
				int count = static_cast<int>(buffer.size());
				float frameTime = static_cast<float>(std::chrono::duration<double>(clock.GetFrameTime()).count()) * 1000.0f;
				buffer.push_back(frameTime);
				auto maxIt = std::max_element(buffer.begin(), buffer.end());
				float maxValue = round(*maxIt);
				std::string label = std::format("0-{} ms", maxValue);
				ImGui::SetNextWindowSize({ 280, 150 }, ImGuiCond_FirstUseEver);
				ImGui::SetNextWindowPos({ 50, 20 }, ImGuiCond_FirstUseEver);
				ImGui::Begin("Debug info");
				ImGui::PlotLines(
					"Frametime",
					buffer.linearize(),
					count,
					0,
					label.c_str(),
					0.0f,
					maxValue,
					ImVec2(0, 80)
				);
				std::string fTimeLabel = std::format("Frame time: {} ms", round(frameTime));
				std::string fpsLabel = std::format("Frame time: {} ms", round(1000 / frameTime));
				ImGui::Text(fTimeLabel.c_str());
				ImGui::Text(fpsLabel.c_str());
				ImGui::End();
			};
		_components.ImGuiElements.Store(EntityRegistry::Instance().GetNew(), window);
	}
}