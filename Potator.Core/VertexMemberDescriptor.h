#pragma once
#include <vector>
#include <string>
#include "leanWin.h"

#define NAMEOF(variable) #variable

namespace Potator
{
	enum DataFormat
	{
		float4,
		float3,
		float2
	};

	struct VertexMemberDescriptor
	{
		std::string Name;
		UINT Offset;
		UINT Stride;
		DataFormat Format;
	};
}