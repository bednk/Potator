#pragma once
#include <Eigen/Dense>

namespace Potator
{
	struct TransformComponent
	{
		Eigen::Matrix4f Local = Eigen::Matrix4f::Identity();
		Eigen::Matrix4f World = Eigen::Matrix4f::Identity();
	};
}