#include "AxisBoundVelocityCommand.h"

Potator::AxisBoundVelocityCommand::AxisBoundVelocityCommand(ComponentStorage<VelocityComponent>& movements) :
	_movements { movements }
{
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
}
