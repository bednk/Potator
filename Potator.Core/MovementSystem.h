#pragma once
#include "ComponentStorage.h"
#include "VelocityComponent.h"
#include "TransformComponent.h"
#include "IFixedStep.h"

namespace Potator
{
	class MovementSystem : public IFixedStep
	{
	public:
		MovementSystem(ComponentStorage<TransformComponent>& transforms, ComponentStorage<VelocityComponent>& movements);
		void SetTickRate(int tickRate) override;
		void Update() override;
	private:
		void OnTransformAdded(Entity entity, const TransformComponent& component);
		void OnVelocityAdded(Entity entity, const VelocityComponent& component);
		void RemoveMovable(Entity entity);
		ComponentStorage<TransformComponent>& _transforms;
		ComponentStorage<VelocityComponent>& _movements;
		float _tickPeriod = 1;
		std::vector<Entity> _movableEntities;
	};

}
