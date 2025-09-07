#include "MovementInputHandler.h"


Potator::MovementInputHandler::MovementInputHandler(CommandDispatcher& commandDispatcher, ComponentStorage<VelocityComponent>& movements, ComponentStorage<TransformComponent>& transforms, Window& window) :
	_entity { NONE_ENTITY },
	_commandDispatcher { commandDispatcher },
	_command { std::make_shared<RelativeVelocityCommand>(movements, transforms) },
	_movements { movements },
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


    _command->LinearVelocity.x() =
        (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS ? -_linerUnitsPerS : 0.f) +
        (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS ? _linerUnitsPerS : 0.f);

    _command->LinearVelocity.y() =
        (glfwGetKey(_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ? -_linerUnitsPerS : 0.f) +
        (glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? _linerUnitsPerS : 0.f);

    _command->LinearVelocity.z() =
        (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS ? -_linerUnitsPerS : 0.f) +
        (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS ? _linerUnitsPerS : 0.f);

    _command->AngularVelocity.x() =
        (glfwGetKey(_window, GLFW_KEY_J) == GLFW_PRESS ? -_angilarRadiansPerS : 0.f) +
        (glfwGetKey(_window, GLFW_KEY_U) == GLFW_PRESS ? _angilarRadiansPerS : 0.f);

    _command->AngularVelocity.y() =
        (glfwGetKey(_window, GLFW_KEY_Y) == GLFW_PRESS ? -_angilarRadiansPerS : 0.f) +
        (glfwGetKey(_window, GLFW_KEY_I) == GLFW_PRESS ? _angilarRadiansPerS : 0.f);

    _command->AngularVelocity.z() =
        (glfwGetKey(_window, GLFW_KEY_K) == GLFW_PRESS ? -_angilarRadiansPerS : 0.f) +
        (glfwGetKey(_window, GLFW_KEY_H) == GLFW_PRESS ? _angilarRadiansPerS : 0.f);

    _commandDispatcher.Enqueue(_entity, _command);
}
