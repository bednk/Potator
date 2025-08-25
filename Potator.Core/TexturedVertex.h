#pragma once
#include <Eigen/Dense>
#include "VertexMemberDescriptor.h"

namespace Potator
{
	struct TexturedVertex
	{
		Eigen::Vector4f Position;
		Eigen::Vector2f Uv;

		static const std::vector<VertexMemberDescriptor> GetLayout()
		{
			return
			{
				{ NAMEOF(Position), offsetof(TexturedVertex, Position), sizeof(Eigen::Vector4f), DataFormat::float4 },
				{ NAMEOF(Uv), offsetof(TexturedVertex, Uv), sizeof(Eigen::Vector2f), DataFormat::float2 }
			};
		}
	};
}
