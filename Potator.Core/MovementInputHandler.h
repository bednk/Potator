#pragma once
#include "CommandDispatcher.h"
#include "IInputHandler.h"
#include <Eigen/Dense>
#include "RelativeVelocityCommand.h"
#include "WindowWrapper.h"

namespace Potator
{
	class MovementInputHandler : public IInputHandler
	{
	public:
		MovementInputHandler(CommandDispatcher& commandDispatcher, ComponentStorage<VelocityComponent>& movements, ComponentStorage<TransformComponent>& transforms, WindowWrapper& window);
		void SetEntity(Entity entity);
		void Handle() override;
	private:
		Entity _entity;
		CommandDispatcher& _commandDispatcher;
		std::shared_ptr<RelativeVelocityCommand> _command;
		ComponentStorage<VelocityComponent>& _movements;
		WindowWrapper& _window;
		float _linerUnitsPerS = 0.05f;
		float _angilarRadiansPerS = 1;
	};
}

