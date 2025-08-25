#pragma once
#include <Eigen/Dense>
#include <VertexMemberDescriptor.h>

namespace Potator
{
	struct Vertex
	{
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