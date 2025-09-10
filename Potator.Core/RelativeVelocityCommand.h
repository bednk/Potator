#pragma once
#include "ICommand.h"
#include "ComponentStorage.h"
#include "VelocityComponent.h"
#include "TransformComponent.h"
#include "PooledObject.h"


namespace Potator
{
	class RelativeVelocityCommand : public ICommand, public PooledObject<RelativeVelocityCommand>
	{
	public:
		static RelativeVelocityCommand* Get(ComponentStorage<VelocityComponent>& movements, ComponentStorage<TransformComponent>& transforms);
		void Execute(Entity entity) override;
		Eigen::Vector3f LinearVelocity = Eigen::Vector3f::Zero();
		Eigen::Vector3f AngularVelocity = Eigen::Vector3f::Zero();
	private:
		RelativeVelocityCommand(ComponentStorage<VelocityComponent>& movements, ComponentStorage<TransformComponent>& transforms);
		ComponentStorage<VelocityComponent>& _movements;
		ComponentStorage<TransformComponent>& _transforms;
	};
}