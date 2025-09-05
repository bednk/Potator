#include "CustomExtension.h"
#include <ControllerMovementInputHandler.h>

Potator::CustomExtension::CustomExtension(Systems& systems, Components& components) :
	_systems { systems },
	_components { components }
{
}

void Potator::CustomExtension::Initialize()
{
	BindCameraController();
	_systems.Loader.Load("teapot.glb");
}

void Potator::CustomExtension::BindCameraController()
{
	Entity camera = _systems.Views.GetActive();
	std::shared_ptr<IInputHandler> controllerHandler = std::make_shared<ControllerMovementInputHandler>(_systems.CommandDispatcher, _components.Movements, _components.Transforms);
	controllerHandler->SetEntity(camera);
	_systems.Views.ViewChanged.connect([this, controllerHandler](Entity e) { controllerHandler->SetEntity(e); });
	_systems.WindowHandler.RegisterInputHandler(controllerHandler);
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
