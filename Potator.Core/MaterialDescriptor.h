#pragma once
#include <Eigen/Dense>

namespace Potator
{
	static const Eigen::Vector4f MISSING_COLOR = { 1.0f, 0.0f, 1.0f, 1.0f };

	struct MaterialDescriptor
	{
		Eigen::Vector4f Color = MISSING_COLOR;
		int HasTexture = 0;
		int HasColor = 0;
	};
}