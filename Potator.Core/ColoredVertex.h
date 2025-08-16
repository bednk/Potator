#pragma once
#include <Eigen/Dense>
#include <VertexMemberDescriptor.h>

namespace Potator
{
	struct ColoredVertex
	{
		ColoredVertex() = default;
		ColoredVertex(const Eigen::Vector4f& position, const Eigen::Vector4f& color) :
			Position { position },
			Color { color } {}
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