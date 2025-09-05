#include "ControllerMovementInputHandler.h"

Potator::ControllerMovementInputHandler::ControllerMovementInputHandler(
	CommandDispatcher& commandDispatcher,
	ComponentStorage<VelocityComponent>& movements,
	ComponentStorage<TransformComponent>& transforms,
	unsigned int joystickId) : _entity(NONE_ENTITY),
	_commandDispatcher{ commandDispatcher },
	_command{ std::make_shared<RelativeVelocityCommand>(movements, transforms) },
	_movements{ movements },
	_joystickId{ joystickId }
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
	if (_entity == NONE_ENTITY || !sf::Joystick::isConnected(_joystickId))
		return;

	_command->LinearVelocity.x() = ApplyDeadzone(sf::Joystick::getAxisPosition(_joystickId, sf::Joystick::Axis::X) / 100.0f) * _linearUnitsPerS;
	_command->LinearVelocity.y() = ApplyDeadzone(sf::Joystick::getAxisPosition(_joystickId, sf::Joystick::Axis::Y) / 100.0f) * _linearUnitsPerS * -1;

	float leftTrigger = ApplyDeadzone(sf::Joystick::getAxisPosition(_joystickId, sf::Joystick::Axis::Z) / 100.0f);
	float rightTrigger = ApplyDeadzone(sf::Joystick::getAxisPosition(_joystickId, sf::Joystick::Axis::R) / 100.0f);
	_command->LinearVelocity.z() = (rightTrigger - leftTrigger) * _linearUnitsPerS;

	_command->AngularVelocity.x() = ApplyDeadzone(sf::Joystick::getAxisPosition(_joystickId, sf::Joystick::Axis::V) / 100.0f) * _angularRadiansPerS;
	_command->AngularVelocity.y() = ApplyDeadzone(sf::Joystick::getAxisPosition(_joystickId, sf::Joystick::Axis::U) / 100.0f) * _angularRadiansPerS;

	bool leftBumper = sf::Joystick::isButtonPressed(_joystickId, 5);
	bool rightBumper = sf::Joystick::isButtonPressed(_joystickId, 4);
	_command->AngularVelocity.z() = (rightBumper ? 1.f : 0.f - (leftBumper ? 1.f : 0.f)) * _angularRadiansPerS;

	_commandDispatcher.Enqueue(_entity, _command);
}