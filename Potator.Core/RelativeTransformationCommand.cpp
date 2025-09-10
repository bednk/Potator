#include "RelativeTransformationCommand.h"

using namespace Potator;

Potator::RelativeTransformationCommand::RelativeTransformationCommand(ComponentStorage<TransformComponent>& transforms) :
	_transforms { transforms }
{
}

RelativeTransformationCommand* Potator::RelativeTransformationCommand::Get(ComponentStorage<TransformComponent>& transforms)
{
    RelativeTransformationCommand* result = _pool.malloc();
    result = new RelativeTransformationCommand(transforms); // pointer managed by pool
    return result;
}

void Potator::RelativeTransformationCommand::Execute(Entity entity)
{
	if (!_transforms.HasComponent(entity))
	{
		return;
	}

	TransformComponent& transform = _transforms[entity];

    Eigen::Affine3f affine = Eigen::Affine3f::Identity();
    if (Scale != Eigen::Vector3f::Zero())
    {
        affine.scale(Scale);
    }

    if (Rotate != Eigen::Vector3f::Zero())
    {
        affine.prerotate(
            Eigen::AngleAxisf(Rotate.z(), Eigen::Vector3f::UnitZ()) *
            Eigen::AngleAxisf(Rotate.y(), Eigen::Vector3f::UnitY()) *
            Eigen::AngleAxisf(Rotate.x(), Eigen::Vector3f::UnitX())
        );
    }

    if (Translate != Eigen::Vector3f::Zero())
    {
        affine.pretranslate(Translate);
    }

    transform.Local *= affine.matrix();

    _pool.destroy(this);
}