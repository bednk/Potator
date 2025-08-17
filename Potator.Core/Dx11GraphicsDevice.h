#pragma once
#include "framework.h"
#include "IGraphicsDevice.h"
#include "DxVertexBuffer.h"

namespace Potator
{
	class Dx11GraphicsDevice : public IGraphicsDevice
	{
	public:
		Dx11GraphicsDevice(HWND windowHandle);
		void Clear(float r, float g, float b, float a) override;
		BufferHandle Create(const IVertexBuffer* buffer) override;
		BufferHandle Create(const IndexBuffer* buffer) override;
		void Bind(BufferHandle buffer) override;
		void Bind(const IShaderBinary* shader) override;
		void Draw(const MeshComponent& mesh) override;
		void Present() override;
	private:
		Microsoft::WRL::ComPtr <ID3D11Device> _device;
		Microsoft::WRL::ComPtr <IDXGISwapChain> _swapChain;
		Microsoft::WRL::ComPtr <ID3D11DeviceContext> _context;
		Microsoft::WRL::ComPtr <ID3D11RenderTargetView> _targetView;
		std::vector<DxVertexBuffer> _vertexBuffers;
		std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> _indexBuffers;




	};
}

