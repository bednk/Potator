#pragma once
#include "PointLightComponent.h"

namespace Potator
{
	struct PointLightsBuffer
	{
		PointLightComponent Lights[16];
		unsigned int Count = 0;
	};
}