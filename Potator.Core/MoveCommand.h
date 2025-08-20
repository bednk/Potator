#pragma once
#include "ICommand.h"
#include "MovementSystem.h"

namespace Potator
{
	class MoveCommand : public ICommand
	{
		
	public:
		MoveCommand(MovementSystem& movementSystem);
		void Execute(Entity entity) override;
		Eigen::Vector3f LinearVelocity = Eigen::Vector3f::Zero();
		Eigen::Vector3f AngularVelocity = Eigen::Vector3f::Zero();
	private:
		MovementSystem& _movementSystem;

	};
}