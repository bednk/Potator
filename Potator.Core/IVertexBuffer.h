#pragma once
#include <vector>
#include "framework.h"
#include "VertexMemberDescriptor.h"
#include "IShaderBinary.h"

namespace Potator
{
	class IVertexBuffer
	{
	public:
		virtual const void* GetData() const = 0;
		virtual UINT GetSize() const = 0;
		virtual UINT GetStride() const = 0;
		virtual const std::vector<VertexMemberDescriptor> GetVertexLayout() const = 0;
		virtual const IShaderBinary* GetVsShaderBinary() const = 0;
		virtual ~IVertexBuffer() = default;
	};
}
