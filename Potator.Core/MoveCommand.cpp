#include "MoveCommand.h"

Potator::MoveCommand::MoveCommand(MovementSystem& movementSystem) :
	_movementSystem { movementSystem }
{
}

void Potator::MoveCommand::Execute(Entity entity)
{
	_movementSystem.SetVelocity(entity, LinearVelocity, AngularVelocity);
}
