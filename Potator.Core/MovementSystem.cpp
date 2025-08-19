#include "MovementSystem.h"

Potator::MovementSystem::MovementSystem(ComponentStorage<TransformComponent>& transforms, ComponentStorage<MovementComponent>& movements) :
	_movements { movements },
	_transforms { transforms }
{
	_movements.ComponentAdded.connect([this](Entity e, const MovementComponent& c) { OnMovementAdded(e, c); });
	_movements.ComponentRemoved.connect([this](Entity e) { RemoveMovable(e); });
	_transforms.ComponentAdded.connect([this](Entity e, const TransformComponent& c) { OnTransformAdded(e, c); });
	_transforms.ComponentRemoved.connect([this](Entity e) { RemoveMovable(e); });
}

void Potator::MovementSystem::SetTickRate(int tickRate)
{
	_tickPeriod = 1.0f / tickRate;
}

void Potator::MovementSystem::Update()
{
	for (size_t i = 0; i < _movableEntities.size(); i++)
	{
		Entity entity = _movableEntities[i];
		TransformComponent& transform = _transforms[entity];
		MovementComponent& movement = _movements[entity];

		Eigen::Vector3f position = transform.Local.block<3, 1>(0, 3);
		Eigen::Matrix3f rotation = transform.Local.block<3, 3>(0, 0);

		position += movement.LinearVelocity;

		Eigen::AngleAxisf rotX(movement.AngularVelocity.x() * _tickPeriod, Eigen::Vector3f::UnitX());
		Eigen::AngleAxisf rotY(movement.AngularVelocity.y() * _tickPeriod, Eigen::Vector3f::UnitY());
		Eigen::AngleAxisf rotZ(movement.AngularVelocity.z() * _tickPeriod, Eigen::Vector3f::UnitZ());

		rotation = (rotZ * rotY * rotX).toRotationMatrix() * rotation;

		transform.Local.setIdentity();
		transform.Local.block<3, 3>(0, 0) = rotation;
		transform.Local.block<3, 1>(0, 3) = position;
	}
}

void Potator::MovementSystem::OnMovementAdded(Entity entity, const MovementComponent& component)
{
	if (!_transforms.HasComponent(entity))
	{
		return;
	}

	_movableEntities.push_back(entity);
}

void Potator::MovementSystem::OnTransformAdded(Entity entity, const TransformComponent& component)
{
	if (!_movements.HasComponent(entity))
	{
		return;
	}

	_movableEntities.push_back(entity);
}

void Potator::MovementSystem::RemoveMovable(Entity entity)
{
	auto idx = std::find(_movableEntities.begin(), _movableEntities.end(), entity);
	if (idx == _movableEntities.end())
	{
		return;
	}

	_movableEntities.erase(idx);
}