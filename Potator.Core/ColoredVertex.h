#pragma once
#include <Eigen/Dense>
#include "VertexMemberDescriptor.h"

namespace Potator
{
	struct ColoredVertex
	{
		Eigen::Vector4f Position;
		Eigen::Vector4f Color;

		static const std::vector<VertexMemberDescriptor> GetLayout()
		{
			return
			{
				{ NAMEOF(Position), offsetof(ColoredVertex, Position), sizeof(Eigen::Vector4f), DataFormat::float4 },
				{ NAMEOF(Color), offsetof(ColoredVertex, Color), sizeof(Eigen::Vector4f), DataFormat::float4 }
			};
		}
	};
}