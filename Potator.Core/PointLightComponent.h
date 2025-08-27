#pragma once
#include <Eigen/Dense>

namespace Potator
{
	struct PointLightComponent
	{
		Eigen::Vector4f Color = Eigen::Vector4f::Ones();
		Eigen::Vector3f Position = Eigen::Vector3f::Zero();
		float QuadraticAtt = 1.0f;
		float LinearAtt = 1.0f;
		float ConstAtt = 1.0f;
		char pad[8];
	};
}