#pragma once
#include <Eigen/Dense>
#include "VertexMemberDescriptor.h"

namespace Potator
{
	struct CompositeVertex
	{
		Eigen::Vector4f Position;
		Eigen::Vector2f Uv;
		Eigen::Vector4f Color;

		static const std::vector<VertexMemberDescriptor> GetLayout()
		{
			return
			{
				{ NAMEOF(Position), offsetof(CompositeVertex, Position), sizeof(Eigen::Vector4f), DataFormat::float4 },
				{ NAMEOF(Uv), offsetof(CompositeVertex, Uv), sizeof(Eigen::Vector2f), DataFormat::float2 },
				{ NAMEOF(Color), offsetof(CompositeVertex, Color), sizeof(Eigen::Vector4f), DataFormat::float4 }
			};
		}
	};
}
