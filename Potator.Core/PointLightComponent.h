#pragma once
#include <Eigen/Dense>

namespace Potator
{
	struct PointLightComponent
	{
		Eigen::Vector4f Color = Eigen::Vector4f::Ones();
		Eigen::Vector3f Position = Eigen::Vector3f::Zero();
		float QuadraticAtt = 0.0007f;
		float LinearAtt = 0.014f;
		float ConstAtt = 1.0f;
		char pad[8] = {};
	};
}