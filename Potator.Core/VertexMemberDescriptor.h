#pragma once
#include <vector>
#include <string>
#include "framework.h"

#define NAMEOF(variable) #variable

namespace Potator
{
	enum DataFormat
	{
		float4,
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