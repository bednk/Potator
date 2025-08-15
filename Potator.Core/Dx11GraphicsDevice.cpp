#include "pch.h"
#include "Dx11GraphicsDevice.h"
#include "HRCheck.h"

using namespace Microsoft::WRL;

Potator::Dx11GraphicsDevice::Dx11GraphicsDevice(HWND windowHandle)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = 0;
	swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = windowHandle;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		_swapChain.GetAddressOf(),
		_device.GetAddressOf(),
		nullptr,
		_context.GetAddressOf()
	);

	ComPtr <ID3D11Resource> backBuffer;
	_swapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &_targetView) >> HrCheck::Instance();
}

void Potator::Dx11GraphicsDevice::Clear(float r, float g, float b, float a)
{
	const float color[]{ r, g, b, a };
	_context->ClearRenderTargetView(_targetView.Get(), color);
}

void Potator::Dx11GraphicsDevice::Bind(const IVertexBuffer* buffer)
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.ByteWidth = buffer->GetSize();
	desc.StructureByteStride = buffer->GetStride();
	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = buffer->GetData();
	ComPtr<ID3D11Buffer> vertexBuffer;
	_device->CreateBuffer(&desc, &data, &vertexBuffer) >> HrCheck::Instance();
	_context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &desc.StructureByteStride, nullptr);
}

void Potator::Dx11GraphicsDevice::Draw(const MeshComponent& mesh)
{
	_context->DrawIndexed(mesh.IndexCount, mesh.StartIndexLocation, 0);
}

void Potator::Dx11GraphicsDevice::Present()
{
	_swapChain->Present(1, 0);
}

void Potator::Dx11GraphicsDevice::Bind(const IShaderBinary* shader)
{
	switch (shader->GetType())
	{
		case ShaderType::Vertex:
		{
			ComPtr<ID3D11VertexShader> vertexShader;
			_device->CreateVertexShader(shader->GetData(), shader->GetSize(), nullptr, &vertexShader) >> HrCheck::Instance();
			_context->VSSetShader(vertexShader.Get(), nullptr, 0);
			break;
		}
	default:
		break;
	}
}
