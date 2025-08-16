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
		VertexBuffer(const std::vector<T>& data, const IShaderBinary* shaderBinary) : _data{ std::move(data) }, _shaderBinary{ shaderBinary } {};
		const void* GetData() const override;
		UINT GetSize() const override;
		UINT GetStride() const override;
		const std::vector<VertexMemberDescriptor> GetVertexLayout() const override;
		const IShaderBinary* GetVsShaderBinary() const override;
	private:
		std::vector<T> _data;
		const IShaderBinary* _shaderBinary;
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
	template<class T>
	inline const IShaderBinary* VertexBuffer<T>::GetVsShaderBinary() const
	{
		return _shaderBinary;
	}
}