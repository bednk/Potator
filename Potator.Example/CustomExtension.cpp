#include "CustomExtension.h"
#include <ControllerMovementInputHandler.h>
#include <RelativeTransformationCommand.h>

Potator::CustomExtension::CustomExtension(Systems& systems, Components& components) :
	_systems { systems },
	_components { components }
{
}

void Potator::CustomExtension::Initialize()
{
	SetupCamera();
	_systems.Loader.Load("teapot.glb");
}

void Potator::CustomExtension::SetupCamera()
{
	Entity camera = _systems.Views.GetActive();
	std::shared_ptr<IInputHandler> controllerHandler = std::make_shared<ControllerMovementInputHandler>(_systems.CommandDispatcher, _components.Movements, _components.Transforms);
	controllerHandler->SetEntity(camera);
	_systems.Views.ViewChanged.connect([this, controllerHandler](Entity e) { controllerHandler->SetEntity(e); });
	_systems.WindowHandler.RegisterInputHandler(controllerHandler);


	std::shared_ptr<RelativeTransformationCommand> aroundY = std::make_shared<RelativeTransformationCommand>(_components.Transforms);
	aroundY->Rotate.y() = -3.14f / 4;
	_systems.CommandDispatcher.Enqueue(camera, aroundY);

	std::shared_ptr<RelativeTransformationCommand> transform = std::make_shared<RelativeTransformationCommand>(_components.Transforms);
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
