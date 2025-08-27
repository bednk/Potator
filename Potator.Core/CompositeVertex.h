#pragma once
#include <Eigen/Dense>
#include "VertexMemberDescriptor.h"

namespace Potator
{
	struct CompositeVertex
	{
		Eigen::Vector4f Position = Eigen::Vector4f::Zero();
		Eigen::Vector4f Color = MISSING_COLOR;
		Eigen::Vector3f Normal = Eigen::Vector3f::Zero();
		Eigen::Vector2f Uv = Eigen::Vector2f::Zero();

		static const std::vector<VertexMemberDescriptor> GetLayout()
		{
			return
			{
				{ NAMEOF(Position), offsetof(CompositeVertex, Position), sizeof(Eigen::Vector4f), DataFormat::float4 },
				{ NAMEOF(Color), offsetof(CompositeVertex, Color), sizeof(Eigen::Vector4f), DataFormat::float4 },
				{ NAMEOF(Normal), offsetof(CompositeVertex, Normal), sizeof(Eigen::Vector3f), DataFormat::float4 },
				{ NAMEOF(Uv), offsetof(CompositeVertex, Uv), sizeof(Eigen::Vector2f), DataFormat::float2 }
			};
		}
	};
}
