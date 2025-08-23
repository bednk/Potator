#pragma once
#include "ComponentStorage.h"
#include "MovementComponent.h"
#include "TransformComponent.h"
#include "IFixedStep.h"

namespace Potator
{
	class MovementSystem : public IFixedStep
	{
	public:
		MovementSystem(ComponentStorage<TransformComponent>& transforms, ComponentStorage<MovementComponent>& movements);
		void SetTickRate(int tickRate) override;
		void Update() override;
		void SetVelocity(Entity entity, const Eigen::Vector3f& linear, const Eigen::Vector3f& angular);
	private:
		void OnTransformAdded(Entity entity, const TransformComponent& component);
		void OnMovementAdded(Entity entity, const MovementComponent& component);
		void RemoveMovable(Entity entity);
		ComponentStorage<TransformComponent>& _transforms;
		ComponentStorage<MovementComponent>& _movements;
		float _tickPeriod = 1;
		std::vector<Entity> _movableEntities;
	};

}
