#pragma once
#include "CommandDispatcher.h"
#include "IInputHandler.h"
#include <Eigen/Dense>
#include "MoveCommand.h"

namespace Potator
{
	class MovementInputHandler : public IInputHandler
	{
	public:
		MovementInputHandler(CommandDispatcher& commandDispatcher, MovementSystem& movementSystem);
		void SetEntity(Entity entity);
		void Handle() override;
	private:
		Entity _entity;
		CommandDispatcher& _commandDispatcher;
		MoveCommand _command;
		float _linerUnitsPerS = 0.05f;
		float _angilarRadiansPerS = 1;
	};
}

