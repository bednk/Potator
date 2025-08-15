#pragma once
#include <vector>
#include "framework.h"


namespace Potator
{
	class IVertexBuffer
	{
	public:
		virtual const void* GetData() const = 0;
		virtual UINT GetSize() const = 0;
		virtual UINT GetStride() const = 0;
		virtual ~IVertexBuffer() = default;
	};
}
