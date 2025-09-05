#pragma once
#include "leanWin.h"
#include "IGraphicsDevice.h"
#include "DxVertexBuffer.h"
#include "WindowHandle.h"


namespace Potator
{
	class Dx11GraphicsDevice : public IGraphicsDevice
	{
	public:
		Dx11GraphicsDevice(WindowHandle windowHandle, LaunchingParams params);
		void Clear(float r, float g, float b, float a) override;
		VertexBufferHandle Create(const IVertexBuffer* buffer) override;
		IndexBufferHandle Create(const IndexBuffer* buffer) override;
		ConstantBufferHandle Create(const IConstantBuffer* buffer) override;
		VertexShaderHandle CreateVertexShader(const IShaderBinary* shaderBinary) override;
		PixelShaderHandle CreatePixelShader(const IShaderBinary* shaderBinary) override;
		InputLayoutHandle CreateInputLayout(const std::vector<VertexMemberDescriptor>& vertexMembers, const IShaderBinary* shaderBin) override;
		ShaderResourceHandle Create2dTexture(const RgbaTextureContainer* source) override;
		StructuredBufferHandle CreateStructuredBuffer(const IConstantBuffer* buffer) override;
		void Bind(const VertexBufferHandle* buffer) override;
		void Bind(const IndexBufferHandle* buffer) override;
		void Bind(const ConstantBufferHandle* buffer, PipelineStage stage, UINT slot) override;
		void Bind(const ShaderResourceHandle* resource, PipelineStage stage, UINT slot) override;
		void Bind(const VertexShaderHandle* shader) override;
		void Bind(const PixelShaderHandle* shader) override;
		void Bind(const InputLayoutHandle* inputLayout) override;
		void Update(const IConstantBuffer* data, const ConstantBufferHandle* gpuHandle) override;
		void Draw(const MeshComponent* mesh, const MaterialComponent* material) override;
		void Present() override;
		void OnWindowResized(unsigned int width, unsigned int height) override;
	private:
		void SetDefaultSampler();
		void InitializeDevice(HWND windowHandle);
		void RecreateRenderTargeView();
		void SetViewport(unsigned int w, unsigned int h);
		void RecreateZBuffer(unsigned int w, unsigned int h);
		Microsoft::WRL::ComPtr<ID3D11Device> _device;
		Microsoft::WRL::ComPtr<IDXGISwapChain> _swapChain;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _targetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _depthStencilView;
		std::vector<DxVertexBuffer> _vertexBuffers;
		std::vector<Microsoft::WRL::ComPtr<ID3D11InputLayout>> _inputLayouts;
		std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> _generalBuffers;
		std::vector<Microsoft::WRL::ComPtr<ID3D11VertexShader>> _vertexShaders;
		std::vector<Microsoft::WRL::ComPtr<ID3D11PixelShader>> _pixelShaders;
		std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> _shaderResources;
	};
}

