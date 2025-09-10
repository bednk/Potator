#include "MovementInputHandler.h"


Potator::MovementInputHandler::MovementInputHandler(CommandDispatcher& commandDispatcher,
        ComponentStorage<VelocityComponent>& movements,
        ComponentStorage<TransformComponent>& transforms,
        IGlfwWindowProvider& window) :
	_entity { NONE_ENTITY },
	_commandDispatcher { commandDispatcher },
	_movements { movements },
    _transforms { transforms },
    _window { window.GetGlfwWindow() }
{
}

void Potator::MovementInputHandler::SetEntity(Entity entity)
{
	_entity = entity;
	if (!_movements.HasComponent(entity))
	{
		VelocityComponent vel;
		_movements.Store(entity, vel);
	}
}

void Potator::MovementInputHandler::Handle()
{
    if (_entity == NONE_ENTITY)
        return;


    RelativeVelocityCommand* command = RelativeVelocityCommand::Get(_movements, _transforms);
    command->LinearVelocity.x() =
        (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS ? -_linerUnitsPerS : 0.f) +
        (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS ? _linerUnitsPerS : 0.f);

    command->LinearVelocity.y() =
        (glfwGetKey(_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ? -_linerUnitsPerS : 0.f) +
        (glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? _linerUnitsPerS : 0.f);

    command->LinearVelocity.z() =
        (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS ? -_linerUnitsPerS : 0.f) +
        (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS ? _linerUnitsPerS : 0.f);

    command->AngularVelocity.x() =
        (glfwGetKey(_window, GLFW_KEY_J) == GLFW_PRESS ? -_angilarRadiansPerS : 0.f) +
        (glfwGetKey(_window, GLFW_KEY_U) == GLFW_PRESS ? _angilarRadiansPerS : 0.f);

    command->AngularVelocity.y() =
        (glfwGetKey(_window, GLFW_KEY_Y) == GLFW_PRESS ? -_angilarRadiansPerS : 0.f) +
        (glfwGetKey(_window, GLFW_KEY_I) == GLFW_PRESS ? _angilarRadiansPerS : 0.f);

    command->AngularVelocity.z() =
        (glfwGetKey(_window, GLFW_KEY_K) == GLFW_PRESS ? -_angilarRadiansPerS : 0.f) +
        (glfwGetKey(_window, GLFW_KEY_H) == GLFW_PRESS ? _angilarRadiansPerS : 0.f);

    _commandDispatcher.Enqueue(_entity, command);
}
