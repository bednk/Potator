#pragma once
#include <Eigen/Dense>

namespace Potator
{
	struct DirectlionalLightComponent
	{
		Eigen::Vector4f Color = Eigen::Vector4f::Ones();
		Eigen::Vector3f Dicection = { -1, -0.5, -0.25 };
		char pad[4] = "lol";
	};
}

