#pragma once
#include "IBuffer.h"

namespace Potator
{
	class IConstantBuffer : public IBuffer
	{
	public:
		~IConstantBuffer() = default;
	};
}