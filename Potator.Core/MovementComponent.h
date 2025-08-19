#pragma once
#include <Eigen/Dense>

namespace Potator
{
	struct MovementComponent
	{
		Eigen::Vector3f LinearVelocity = Eigen::Vector3f::Zero();
		Eigen::Vector3f AngularVelocity = Eigen::Vector3f::Zero();
	};
}