#pragma once
#include "leanWin.h"
#include "IVertexBuffer.h"


namespace Potator
{
	template<class T>
	class VertexBuffer : public IVertexBuffer
	{
	public:
		VertexBuffer() = default;
		VertexBuffer(const std::vector<T>& data) : _data{ std::move(data) } {};
		const void* GetData() const override;
		UINT GetSize() const override;
		UINT GetStride() const override;
	private:
		std::vector<T> _data;
	};

	template<class T>
	inline const void* VertexBuffer<T>::GetData() const
	{
		return _data.data();
	}

	template<class T>
	inline UINT VertexBuffer<T>::GetSize() const
	{
		return GetStride() * (UINT)_data.size();
	}

	template<class T>
	inline UINT VertexBuffer<T>::GetStride() const
	{
		return sizeof(T);
	}
}