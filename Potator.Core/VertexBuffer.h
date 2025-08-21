#pragma once
#include "IVertexBuffer.h"
#include "framework.h"

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
		const std::vector<VertexMemberDescriptor> GetVertexLayout() const override;
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
		return GetStride() * _data.size();
	}

	template<class T>
	inline UINT VertexBuffer<T>::GetStride() const
	{
		return sizeof(T);
	}
	template<class T>
	inline const std::vector<VertexMemberDescriptor> VertexBuffer<T>::GetVertexLayout() const
	{
		return T::GetLayout();
	}
}