#pragma once
#include "MeshComponent.h"
#include "IVertexBuffer.h"
#include "IShaderBinary.h"
#include "IndexBuffer.h"
#include "BufferHandle.h"

namespace Potator
{
	class IGraphicsDevice
	{
	public:
		virtual void Clear(float r, float g, float b, float a) = 0;
		virtual BufferHandle Create(const IVertexBuffer* buffer) = 0;
		virtual BufferHandle Create(const IndexBuffer* buffer) = 0;
		virtual void Bind(BufferHandle buffer) = 0;
		virtual void Bind(const IShaderBinary* shader) = 0;
		virtual void Draw(const MeshComponent& mesh) = 0;
		virtual void Present() = 0;
		~IGraphicsDevice() = default;
	};
}