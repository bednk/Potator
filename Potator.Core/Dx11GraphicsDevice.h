#pragma once
#include "framework.h"
#include "IGraphicsDevice.h"
#include <d3d11.h>
#include <wrl/client.h>

namespace Potator
{
	class Dx11GraphicsDevice : public IGraphicsDevice
	{
	public:
		Dx11GraphicsDevice(HWND windowHandle);
		void Clear(float r, float g, float b, float a) override;
		void Bind(const IVertexBuffer* buffer) override;
		void Bind(const IShaderBinary* shader) override;
		void Draw(const MeshComponent& mesh) override;
		void Present() override;
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> _device;
		Microsoft::WRL::ComPtr <IDXGISwapChain> _swapChain;
		Microsoft::WRL::ComPtr <ID3D11DeviceContext> _context;
		Microsoft::WRL::ComPtr <ID3D11RenderTargetView> _targetView;
	};
}

