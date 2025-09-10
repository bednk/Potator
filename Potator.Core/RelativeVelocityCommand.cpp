#include "RelativeVelocityCommand.h"

using namespace Potator;

Potator::RelativeVelocityCommand::RelativeVelocityCommand(ComponentStorage<VelocityComponent>& movements, ComponentStorage<TransformComponent>& transforms) :
	_movements { movements },
	_transforms { transforms }
{
}

RelativeVelocityCommand* Potator::RelativeVelocityCommand::Get(ComponentStorage<VelocityComponent>& movements, ComponentStorage<TransformComponent>& transforms)
{
	RelativeVelocityCommand* result = _pool.malloc();
	result = new RelativeVelocityCommand(movements, transforms); // pointer managed by pool
	return result;
}

void Potator::RelativeVelocityCommand::Execute(Entity entity)
{
	if (!_movements.HasComponent(entity) || !_transforms.HasComponent(entity))
	{
		return;
	}

	auto& transform = _transforms[entity];

	Eigen::Matrix3f rotation = transform.World.block<3, 3>(0, 0);
	rotation.normalize();

	auto& velocity = _movements[entity];
	velocity.Linear = rotation * LinearVelocity;
	velocity.Angular = rotation * AngularVelocity;

	_pool.destroy(this);
}