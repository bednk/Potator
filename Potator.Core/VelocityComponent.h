#pragma once
#include <Eigen/Dense>

namespace Potator
{
	struct VelocityComponent
	{
		Eigen::Vector3f Linear = Eigen::Vector3f::Zero();
		Eigen::Vector3f Angular = Eigen::Vector3f::Zero();
	};
}