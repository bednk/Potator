#pragma once
#include "IBuffer.h"
#include <vector>

namespace Potator
{
	template <class T>
	class Buffer : public IBuffer
	{
	public:
		Buffer(const std::vector<T>& data) : _data{ data } {};
		virtual const void* GetData() const override { return _data.data(); };
		UINT GetSize() const override  { return _data.size() * GetStride(); };
		UINT GetStride() const override  { return sizeof(T); };
	protected:
		std::vector<T> _data;
	};
}