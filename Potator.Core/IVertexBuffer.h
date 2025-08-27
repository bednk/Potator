#pragma once
#include <vector>
#include "IBuffer.h"
#include "VertexMemberDescriptor.h"
#include "IShaderBinary.h"

namespace Potator
{
	class IVertexBuffer : public IBuffer
	{
	public:
		virtual ~IVertexBuffer() = default;
	};
}
