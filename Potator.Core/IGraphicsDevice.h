#pragma once
#include "MeshComponent.h"
#include "IVertexBuffer.h"
#include "IShaderBinary.h"

namespace Potator
{
	class IGraphicsDevice
	{
	public:
		virtual void Clear(float r, float g, float b, float a) = 0;
		virtual void Bind(const IVertexBuffer* buffer) = 0;
		virtual void Bind(const IShaderBinary* shader) = 0;
		virtual void Draw(const MeshComponent& mesh) = 0;
		virtual void Present() = 0;
		~IGraphicsDevice() = default;
	};
}