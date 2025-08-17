#pragma once
#include <vector>
#include "IBuffer.h"

namespace Potator
{
	class IndexBuffer : public IBuffer
	{
	public:
		IndexBuffer() = default;
		IndexBuffer(const std::vector<unsigned short>& data, UINT offset = 0) : _data { data }, _offset{offset} {};
		virtual const void* GetData() const override { return _data.data(); };
		UINT GetSize() const override { return _data.size() * GetStride(); };
		UINT GetStride() const override { return sizeof(unsigned short); };
		UINT GetOffset() const { return _offset; };
	private:
		UINT _offset;
		std::vector<unsigned short> _data;
	};
}