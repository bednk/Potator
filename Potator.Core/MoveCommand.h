#pragma once
#include "ICommand.h"
#include "ComponentStorage.h"
#include "MovementComponent.h"

namespace Potator
{
	class MoveCommand : public ICommand
	{
		
	public:
		MoveCommand(ComponentStorage<MovementComponent>& movements);
		void Execute(Entity entity) override;
		Eigen::Vector3f LinearVelocity = Eigen::Vector3f::Zero();
		Eigen::Vector3f AngularVelocity = Eigen::Vector3f::Zero();
	private:
		ComponentStorage<MovementComponent>& _movements;

	};
}