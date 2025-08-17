#pragma once
#include <vector>
#include "Buffer.h"

namespace Potator
{
	class IndexBuffer : public Buffer<unsigned short>
	{
	public:
		IndexBuffer() = default;
		IndexBuffer(const std::vector<unsigned short>& data, UINT offset = 0) : Buffer{ data }, _offset{ offset } {};
		UINT GetOffset() const { return _offset; };
	private:
		UINT _offset;
	};
}