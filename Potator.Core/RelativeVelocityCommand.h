#pragma once
#include "ICommand.h"
#include "ComponentStorage.h"
#include "VelocityComponent.h"
#include "TransformComponent.h"

namespace Potator
{
	class RelativeVelocityCommand : public ICommand
	{
	public:
		RelativeVelocityCommand(ComponentStorage<VelocityComponent>& movements, ComponentStorage<TransformComponent>& transforms);
		void Execute(Entity entity) override;
		Eigen::Vector3f LinearVelocity = Eigen::Vector3f::Zero();
		Eigen::Vector3f AngularVelocity = Eigen::Vector3f::Zero();
	private:
		ComponentStorage<VelocityComponent>& _movements;
		ComponentStorage<TransformComponent>& _transforms;
	};
}