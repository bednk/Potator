#pragma once
#include "IConstantBuffer.h"
#include <vector>

namespace Potator
{
	template <class T>
	class ConstantBuffer : public IConstantBuffer
	{
	public:
		ConstantBuffer(const T& data) : _data{ data } {};
		const void* GetData() const override { return &_data; };
		UINT GetSize() const override { return GetStride(); };
		UINT GetStride() const override { return sizeof(T); };
		void Update(const T& data) { _data = data; }
	protected:
		T _data;
	};
}