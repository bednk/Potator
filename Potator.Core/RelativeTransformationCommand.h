#pragma once
#include "ICommand.h"
#include "ComponentStorage.h"
#include "TransformComponent.h"
#include "PooledObject.h"


namespace Potator
{
	class RelativeTransformationCommand : public ICommand, public PooledObject<RelativeTransformationCommand>
	{
	public:
		static RelativeTransformationCommand* Get(ComponentStorage<TransformComponent>& transforms);
		void Execute(Entity entity) override;
		Eigen::Vector3f Translate = Eigen::Vector3f::Zero();
		Eigen::Vector3f Rotate = Eigen::Vector3f::Zero();
		Eigen::Vector3f Scale = Eigen::Vector3f::Zero();
	private:
		RelativeTransformationCommand(ComponentStorage<TransformComponent>& transforms);
		ComponentStorage<TransformComponent>& _transforms;
	};
}