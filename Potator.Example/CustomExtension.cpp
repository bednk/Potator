#include "CustomExtension.h"
#include <ControllerMovementInputHandler.h>
#include <RelativeTransformationCommand.h>
#include "EntityRegistry.h"
#include "imgui.h"

Potator::CustomExtension::CustomExtension(Systems& systems, Components& components) :
	_systems { systems },
	_components { components }
{
}

void Potator::CustomExtension::Initialize()
{
	SetupCamera();
	_systems.Loader.Load("teapot.glb");

	Entity controllEntity = EntityRegistry::Instance().GetNew();
	ImGuiComponent controll;
	LightsConfig& lights = _systems.Lighting.GetConfig();
	
	controll.Draw = [&lights]()
		{
			ImGui::SetNextWindowSize({ 300, 100 }, ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowPos({ 50, 200 }, ImGuiCond_FirstUseEver);
			ImGui::Begin("Enviroment lights");
			ImGui::Text("Colors");
			ImGui::ColorEdit4("Directional", lights.Directional.Color.data());
			ImGui::ColorEdit4("Ambient", lights.Ambient.Color.data());
			ImGui::End();
		};
	_components.ImGuiElements.Store(controllEntity, controll);
}

void Potator::CustomExtension::SetupCamera()
{
	Entity camera = _systems.Views.GetActive();
	std::shared_ptr<IInputHandler> controllerHandler = std::make_shared<ControllerMovementInputHandler>(_systems.CommandDispatcher, _components.Movements, _components.Transforms);
	controllerHandler->SetEntity(camera);
	_systems.Views.ViewChanged.connect([this, controllerHandler](Entity e) { controllerHandler->SetEntity(e); });
	_systems.WindowHandler.RegisterInputHandler(controllerHandler);

	RelativeTransformationCommand* aroundY = RelativeTransformationCommand::Get(_components.Transforms);
	aroundY->Rotate.y() = -3.14f / 4;
	_systems.CommandDispatcher.Enqueue(camera, aroundY);

	RelativeTransformationCommand* transform = RelativeTransformationCommand::Get(_components.Transforms);
	transform->Rotate.x() = 3.14f / 4;
	transform->Translate = { 0.0f, 120.0f, -120.0f };
	_systems.CommandDispatcher.Enqueue(camera, transform);
}

void Potator::CustomExtension::OnFrameStarted()
{
	//pass
}

void Potator::CustomExtension::OnBeforeStateUpdated()
{
	//pass
}

void Potator::CustomExtension::OnBeforeSceneRendered()
{
	//pass
}

void Potator::CustomExtension::Cleanup()
{
	//pass
}
