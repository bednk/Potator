#include "ControllerMovementInputHandler.h"
#include <GLFW/glfw3.h>

Potator::ControllerMovementInputHandler::ControllerMovementInputHandler(
	CommandDispatcher& commandDispatcher,
	ComponentStorage<VelocityComponent>& movements,
	ComponentStorage<TransformComponent>& transforms,
	unsigned int joystickId) : _entity(NONE_ENTITY),
	_commandDispatcher{ commandDispatcher },
	_movements { movements },
	_joystickId { joystickId },
    _transforms { transforms }
{
}

void Potator::ControllerMovementInputHandler::SetEntity(Entity entity)
{
	_entity = entity;
	if (!_movements.HasComponent(entity))
	{
		VelocityComponent vel;
		_movements.Store(entity, vel);
	}
}

float Potator::ControllerMovementInputHandler::ApplyDeadzone(float value, float deadzone)
{
	if (std::abs(value) < deadzone)
		return 0.f;
	return value;
}

void Potator::ControllerMovementInputHandler::Handle()
{
    if (_entity == NONE_ENTITY || !glfwJoystickPresent(_joystickId))
    {
        return;
    }

    int axisCount = 0;
    const float* axes = glfwGetJoystickAxes(_joystickId, &axisCount);

    int buttonCount = 0;
    const unsigned char* buttons = glfwGetJoystickButtons(_joystickId, &buttonCount);

    if (!axes || !buttons)
    {
        return;
    }

    auto command = RelativeVelocityCommand::Get(_movements, _transforms);
    command->LinearVelocity.x() = ApplyDeadzone(axes[0]) * _linearUnitsPerS;
    command->LinearVelocity.y() = ApplyDeadzone(-axes[1]) * _linearUnitsPerS;

    float leftTrigger = (axisCount > 4) ? ApplyDeadzone((axes[4] + 1.f) / 2.f) : 0.f;
    float rightTrigger = (axisCount > 5) ? ApplyDeadzone((axes[5] + 1.f) / 2.f) : 0.f;
    command->LinearVelocity.z() = (rightTrigger - leftTrigger) * _linearUnitsPerS;

    command->AngularVelocity.y() = (axisCount > 2) ? ApplyDeadzone(axes[2]) * _angularRadiansPerS : 0.f;
    command->AngularVelocity.x() = (axisCount > 3) ? ApplyDeadzone(axes[3]) * _angularRadiansPerS : 0.f;

    bool leftBumper = (buttonCount > 5) && buttons[5];
    bool rightBumper = (buttonCount > 4) && buttons[4];
    command->AngularVelocity.z() = (rightBumper ? 1.f : 0.f - (leftBumper ? 1.f : 0.f)) * _angularRadiansPerS;

    _commandDispatcher.Enqueue(_entity, command);
}