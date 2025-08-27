#pragma once
#include "Eigen/Dense"

namespace Potator
{
	struct AmbientLightComponent
	{
		Eigen::Vector4f Color = { 0.05f, 0.05f, 0.05f, 1.0f };
	};
}