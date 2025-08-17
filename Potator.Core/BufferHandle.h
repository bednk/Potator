#pragma once

namespace Potator
{
	enum class BufferType
	{
		Vertex,
		Index,
		Constant
	};

	struct ConstantBufferHandle { size_t Id; };
	struct VertexBufferHandle { size_t Id; };
	struct IndexBufferHandle { size_t Id; };
}