#pragma once

namespace Potator
{
	enum class BufferType
	{
		Vertex,
		Index,
		Constant
	};

	struct BufferHandle
	{
		size_t Handle;
		BufferType Type;
	};
}