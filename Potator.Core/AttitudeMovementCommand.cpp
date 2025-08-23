#include "AttitudeMovementCommand.h"

Potator::AttitudeMovementCommnand::AttitudeMovementCommnand(ComponentStorage<MovementComponent>& movements, ComponentStorage<TransformComponent>& transforms) :
	_movements { movements },
	_transforms { transforms }
{
}

void Potator::AttitudeMovementCommnand::Execute(Entity entity)
{
	if (!_movements.HasComponent(entity) || !_transforms.HasComponent(entity))
	{
		return;
	}

	auto& movemenet = _movements[entity];
	auto& transform = _transforms[entity];

	Eigen::Matrix3f basis = transform.World.block<3, 3>(0, 0);

	Eigen::Vector3f right = basis.col(0).normalized();
	Eigen::Vector3f up = basis.col(1).normalized();
	Eigen::Vector3f forward = basis.col(2).normalized();

	Eigen::Matrix3f rotation;
	rotation.col(0) = right;
	rotation.col(1) = up;
	rotation.col(2) = forward;

	movemenet.LinearVelocity = rotation * LinearVelocity;
	movemenet.AngularVelocity = rotation * AngularVelocity;
}
