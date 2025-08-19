#pragma once
#include <Eigen/Dense>

namespace Potator
{
	struct MovementComponent
	{
		Eigen::Vector4f LinearVelocity = Eigen::Vector4f::Zero();
		Eigen::Vector4f AngularVelocity = Eigen::Vector4f::Zero();
	};
}