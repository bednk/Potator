#pragma once
#include "ICommand.h"
#include "ComponentStorage.h"
#include "TransformComponent.h"

namespace Potator
{
	class RelativeTransformationCommand : public ICommand
	{
	public:
		RelativeTransformationCommand(ComponentStorage<TransformComponent>& transforms);
		void Execute(Entity entity) override;
		Eigen::Vector3f Translate = Eigen::Vector3f::Zero();
		Eigen::Vector3f Rotate = Eigen::Vector3f::Zero();
		Eigen::Vector3f Scale = Eigen::Vector3f::Zero();
	private:
		ComponentStorage<TransformComponent>& _transforms;
	};
}