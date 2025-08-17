#pragma once
#include  "framework.h"
#include "BufferHandle.h"

namespace Potator
{
	struct MeshComponent
	{
		BufferHandle VertexBuffer;
		BufferHandle IndexBuffer;
		UINT IndexCount = 0;
		UINT StartIndexLocation = 0;
		UINT VertexOffset = 0;
	};
}

