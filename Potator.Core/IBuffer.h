#pragma once
#include "leanWin.h"

namespace Potator
{
	class IBuffer
	{
	public:
		virtual const void* GetData() const = 0;
		virtual UINT GetSize() const = 0;
		virtual UINT GetStride() const = 0;
		virtual ~IBuffer() = default;
	};
}