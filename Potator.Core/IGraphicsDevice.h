#pragma once
#include "MeshComponent.h"
#include "IVertexBuffer.h"
#include "IConstantBuffer.h"
#include "IShaderBinary.h"
#include "IndexBuffer.h"
#include "BufferHandle.h"
#include "PipelineStage.h"
#include "TransformComponent.h"

namespace Potator
{
	enum class GpuApi
	{
		Dx11,
		OpenGL
	};

	class IGraphicsDevice
	{
	public:
		virtual void Clear(float r, float g, float b, float a) = 0;
		virtual VertexBufferHandle Create(const IVertexBuffer* buffer) = 0;
		virtual IndexBufferHandle Create(const IndexBuffer* buffer) = 0;
		virtual ConstantBufferHandle Create(const IConstantBuffer* buffer) = 0;
		virtual void Bind(const VertexBufferHandle* buffer) = 0;
		virtual void Bind(const IndexBufferHandle* buffer) = 0;
		virtual void Bind(const ConstantBufferHandle* buffer, PipelineStage stage, UINT slot) = 0;
		virtual void Bind(const IShaderBinary* shader) = 0;
		virtual void Update(const IConstantBuffer* data, const ConstantBufferHandle* gpuHandle) = 0;
		virtual void Draw(const MeshComponent* mesh) = 0;
		virtual void Present() = 0;
		~IGraphicsDevice() = default;
	};
}