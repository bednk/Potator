#include "AxisBoundVelocityCommand.h"

using namespace Potator;

Potator::AxisBoundVelocityCommand::AxisBoundVelocityCommand(ComponentStorage<VelocityComponent>& movements) :
	_movements { movements }
{
}

AxisBoundVelocityCommand* Potator::AxisBoundVelocityCommand::Get(ComponentStorage<VelocityComponent>& movements)
{
	AxisBoundVelocityCommand* result = _pool.malloc();
	result = new AxisBoundVelocityCommand(movements); // pointer managed by pool
	return result;
}

void Potator::AxisBoundVelocityCommand::Execute(Entity entity)
{
	if (!_movements.HasComponent(entity))
	{
		return;
	}

	auto& movement = _movements[entity];
	movement.Linear = LinearVelocity;
	movement.Angular = AngularVelocity;

	_pool.destroy(this);
}
