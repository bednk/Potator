#pragma once
#include <Eigen/Dense>
#include "Export.h"

namespace Potator
{
	struct EX TransformComponent
	{
		Eigen::Matrix4f Local;
		Eigen::Matrix4f World;
	};
}