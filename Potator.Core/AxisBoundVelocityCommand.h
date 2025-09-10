#pragma once
#include "ICommand.h"
#include "ComponentStorage.h"
#include "VelocityComponent.h"
#include <boost/pool/object_pool.hpp>
#include "PooledObject.h"

namespace Potator
{
	class AxisBoundVelocityCommand : public ICommand, public PooledObject<AxisBoundVelocityCommand>
	{
	friend class boost::object_pool<AxisBoundVelocityCommand>;
	public:
		static AxisBoundVelocityCommand* Get(ComponentStorage<VelocityComponent>& movements);
		void Execute(Entity entity) override;
		Eigen::Vector3f LinearVelocity = Eigen::Vector3f::Zero();
		Eigen::Vector3f AngularVelocity = Eigen::Vector3f::Zero();
	private:
		AxisBoundVelocityCommand(ComponentStorage<VelocityComponent>& movements);
		ComponentStorage<VelocityComponent>& _movements;
	};
}