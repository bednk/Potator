#pragma once
#include <Eigen/Dense>
#include <VertexMemberDescriptor.h>

namespace Potator
{
	struct Vertex
	{
		Vertex() = default;
		Vertex(float x, float y, float z, float w = 1.0f)
			: Position(Eigen::Vector4f(x, y, z, w))
		{
		}
		Eigen::Vector4f Position;

		static const std::vector<VertexMemberDescriptor> GetLayout()
		{
			return
			{
				{ NAMEOF(Position), offsetof(Vertex, Position), sizeof(Eigen::Vector4f), DataFormat::float4 }
			};
		}
	};
}