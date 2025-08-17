#pragma once
#include "framework.h"

namespace Potator
{
	class ICpuBuffer
	{
	public:
		virtual const void* GetData() const = 0;
		virtual UINT GetSize() const = 0;
		virtual UINT GetStride() const = 0;
		virtual ~ICpuBuffer() = default;
	};
}