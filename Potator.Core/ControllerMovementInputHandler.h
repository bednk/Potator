#pragma once
#include "CommandDispatcher.h"
#include "IInputHandler.h"
#include <Eigen/Dense>
#include "AttitudeMovementCommand.h"
#include <SFML/Window/Joystick.hpp>

namespace Potator
{
	class ControllerMovementInputHandler : public IInputHandler
	{
	public:
		ControllerMovementInputHandler(CommandDispatcher& commandDispatcher,
			ComponentStorage<MovementComponent>& movements,
			ComponentStorage<TransformComponent>& transforms,
			unsigned int joystickId = 0);

		void SetEntity(Entity entity);
		void Handle() override;

	private:
		Entity _entity;
		CommandDispatcher& _commandDispatcher;
		AttitudeMovementCommnand _command;
		ComponentStorage<MovementComponent>& _movements;
		unsigned int _joystickId;

		float _linearUnitsPerS = 0.05f;
		float _angularRadiansPerS = 1.0f;

		float ApplyDeadzone(float value, float deadzone = 0.1f);
	};
}