#pragma once
#include "ICommand.h"
#include "ComponentStorage.h"
#include "MovementComponent.h"
#include "TransformComponent.h"

namespace Potator
{
	class AttitudeMovementCommnand : public ICommand
	{
	public:
		AttitudeMovementCommnand(ComponentStorage<MovementComponent>& movements, ComponentStorage<TransformComponent>& transforms);
		void Execute(Entity entity) override;
		Eigen::Vector3f LinearVelocity = Eigen::Vector3f::Zero();
		Eigen::Vector3f AngularVelocity = Eigen::Vector3f::Zero();
	private:
		ComponentStorage<MovementComponent>& _movements;
		ComponentStorage<TransformComponent>& _transforms;
	};
}