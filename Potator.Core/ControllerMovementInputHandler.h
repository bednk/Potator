#pragma once
#include "CommandDispatcher.h"
#include "IInputHandler.h"
#include <Eigen/Dense>
#include "RelativeVelocityCommand.h"
#include <SFML/Window/Joystick.hpp>

namespace Potator
{
	class ControllerMovementInputHandler : public IInputHandler
	{
	public:
		ControllerMovementInputHandler(CommandDispatcher& commandDispatcher,
			ComponentStorage<VelocityComponent>& movements,
			ComponentStorage<TransformComponent>& transforms,
			unsigned int joystickId = 0);

		void SetEntity(Entity entity) override;
		void Handle() override;

	private:
		Entity _entity;
		CommandDispatcher& _commandDispatcher;
		std::shared_ptr<RelativeVelocityCommand> _command;
		ComponentStorage<VelocityComponent>& _movements;
		unsigned int _joystickId;

		float _linearUnitsPerS = 0.5f;
		float _angularRadiansPerS = 1.0f;

		float ApplyDeadzone(float value, float deadzone = 0.1f);
	};
}