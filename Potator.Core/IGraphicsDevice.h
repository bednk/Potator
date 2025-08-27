#pragma once
#include "MeshComponent.h"
#include "IVertexBuffer.h"
#include "IConstantBuffer.h"
#include "IShaderBinary.h"
#include "IndexBuffer.h"
#include "BufferHandle.h"
#include "PipelineStage.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"
#include "LaunchingParams.h"
#include "RgbaTextureContainer.h"

namespace Potator
{
	class IGraphicsDevice
	{
	public:
		virtual void Clear(float r, float g, float b, float a) = 0;
		virtual VertexBufferHandle Create(const IVertexBuffer* buffer) = 0;
		virtual IndexBufferHandle Create(const IndexBuffer* buffer) = 0;
		virtual ConstantBufferHandle Create(const IConstantBuffer* buffer) = 0;
		virtual VertexShaderHandle CreateVertexShader(const IShaderBinary* shaderBinary) = 0;
		virtual PixelShaderHandle CreatePixelShader(const IShaderBinary* shaderBinary) = 0;
		virtual InputLayoutHandle CreateInputLayout(const std::vector<VertexMemberDescriptor>& vertexMembers, const IShaderBinary* shaderBin) = 0;
		virtual ShaderResourceHandle Create2dTexture(const RgbaTextureContainer* source) = 0;
		virtual StructuredBufferHandle CreateStructuredBuffer(const IConstantBuffer* buffer) = 0;
		virtual void Bind(const VertexBufferHandle* buffer) = 0;
		virtual void Bind(const IndexBufferHandle* buffer) = 0;
		virtual void Bind(const ConstantBufferHandle* buffer, PipelineStage stage, UINT slot) = 0;
		virtual void Bind(const ShaderResourceHandle* resource, PipelineStage stage, UINT slot) = 0;
		virtual void Bind(const VertexShaderHandle* shader) = 0;
		virtual void Bind(const PixelShaderHandle* shader) = 0;
		virtual void Bind(const InputLayoutHandle* inputLayout) = 0;
		virtual void Update(const IConstantBuffer* data, const ConstantBufferHandle* gpuHandle) = 0;
		virtual void Draw(const MeshComponent* mesh, const MaterialComponent* material) = 0;
		virtual void Present() = 0;
		virtual void OnWindowResized(unsigned int width, unsigned int height) = 0;
		~IGraphicsDevice() = default;
	};
}