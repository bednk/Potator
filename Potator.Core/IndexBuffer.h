#pragma once
#include <vector>

namespace Potator
{
	class IndexBuffer
	{
	public:
		IndexBuffer() = default;
		IndexBuffer(const std::vector<unsigned short>& data, UINT offset = 0) : _data{ data }, _offset{ offset } {};
		UINT GetSize() const { return _data.size() * GetStride(); };
		UINT GetStride() const { return sizeof(unsigned short); };
		UINT GetOffset() const { return _offset; };
		virtual const void* GetData() const { return _data.data(); };
	private:
		std::vector<unsigned short> _data;
		UINT _offset;
	};
}
