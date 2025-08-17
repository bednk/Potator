#pragma once
#include "IConstantBuffer.h"
#include <vector>

namespace Potator
{
	template <class T>
	class ConstantBuffer : public IConstantBuffer
	{
	public:
		ConstantBuffer(const T data) : _data{ data } {};
		virtual const void* GetData() const override { return &_data; };
		UINT GetSize() const override  { return GetStride(); };
		UINT GetStride() const override  { return sizeof(T); };
	protected:
		T _data;
	};
}