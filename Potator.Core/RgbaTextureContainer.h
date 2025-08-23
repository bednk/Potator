#pragma once
#include<vector>
#include <Eigen/dense>

namespace Potator
{
	struct RgbaTextureContainer
	{
		int Width = 0;
		int Height = 0;
		std::vector<unsigned char> Pixels;
	};
}