#pragma once
#include "CommandDispatcher.h"
#include "IInputHandler.h"
#include <Eigen/Dense>
#include "RelativeVelocityCommand.h"

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
		ComponentStorage<VelocityComponent>& _movements;
		ComponentStorage<TransformComponent>& _transforms;
		unsigned int _joystickId;

		float _linearUnitsPerS = 0.5f;
		float _angularRadiansPerS = 1.0f;

		float ApplyDeadzone(float value, float deadzone = 0.1f);
	};
}