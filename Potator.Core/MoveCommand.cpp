#include "MoveCommand.h"

Potator::MoveCommand::MoveCommand(ComponentStorage<MovementComponent>& movements) :
	_movements { movements }
{
}

void Potator::MoveCommand::Execute(Entity entity)
{
	if (!_movements.HasComponent(entity))
	{
		return;
	}

	auto& movement = _movements[entity];
	movement.LinearVelocity = LinearVelocity;
	movement.AngularVelocity = AngularVelocity;
}
