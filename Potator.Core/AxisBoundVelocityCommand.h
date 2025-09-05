#pragma once
#include "ICommand.h"
#include "ComponentStorage.h"
#include "VelocityComponent.h"

namespace Potator
{
	class AxisBoundVelocityCommand : public ICommand
	{
		
	public:
		AxisBoundVelocityCommand(ComponentStorage<VelocityComponent>& movements);
		void Execute(Entity entity) override;
		Eigen::Vector3f LinearVelocity = Eigen::Vector3f::Zero();
		Eigen::Vector3f AngularVelocity = Eigen::Vector3f::Zero();
	private:
		ComponentStorage<VelocityComponent>& _movements;

	};
}