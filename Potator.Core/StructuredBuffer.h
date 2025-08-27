#pragma once
#include "IConstantBuffer.h"
#include <vector>

namespace Potator
{
	template <class T, size_t N = 1>
	class StructuredBuffer : public IConstantBuffer
	{
	public:
		StructuredBuffer(const T(&data)[N])
		{
			for (size_t i = 0; i < N; ++i)
				_data[i] = data[i];
		}
		const void* GetData() const override { return &_data; };
		UINT GetSize() const override { return GetStride() * N; };
		UINT GetStride() const override { return sizeof(T); };
		void Update(const T(&data)[N])
		{
			for (size_t i = 0; i < N; ++i)
				_data[i] = data[i];
		}
	protected:
		T _data[N];
	};
}
