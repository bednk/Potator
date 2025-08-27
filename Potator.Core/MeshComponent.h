#pragma once
#include  "framework.h"
#include "BufferHandle.h"
#include "IShaderBinary.h"

namespace Potator
{
	struct MeshComponent
	{
		VertexBufferHandle VertexBuffer = { ~0u };
		IndexBufferHandle IndexBuffer = { ~0u };
		UINT IndexCount = 0;
		UINT StartIndexLocation = 0;
	};
}

