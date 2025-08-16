#include "pch.h"
#include "Dx11GraphicsDevice.h"
#include "HRCheck.h"
#include "Exception.h"
#include "DxDescriptorsConverter.h"


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

	ComPtr<ID3D11Resource> backBuffer;
	_swapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &_targetView) >> HrCheck::Instance();
	_context->OMSetRenderTargets(1, _targetView.GetAddressOf(), nullptr);
	D3D11_VIEWPORT viewPort = {};
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.MaxDepth = 1;
	viewPort.MinDepth = 0;
	viewPort.Width = 800;
	viewPort.Height = 600;
	_context->RSSetViewports(1, &viewPort);
	_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Potator::Dx11GraphicsDevice::Clear(float r, float g, float b, float a)
{
	const float color[]{ r, g, b, a };
	_context->ClearRenderTargetView(_targetView.Get(), color);
}

void Potator::Dx11GraphicsDevice::Bind(const IVertexBuffer* buffer)
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.ByteWidth = buffer->GetSize();
	bufferDesc.StructureByteStride = buffer->GetStride();
	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = buffer->GetData();
	ComPtr<ID3D11Buffer> vertexBuffer;
	_device->CreateBuffer(&bufferDesc, &data, &vertexBuffer) >> HrCheck::Instance();
	UINT _ = 0;
	_context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &bufferDesc.StructureByteStride, &_);

	auto vertexMembers = buffer->GetVertexLayout();
	auto vertexDesc = std::make_unique<D3D11_INPUT_ELEMENT_DESC[]>(vertexMembers.size());
	for (size_t i = 0; i < vertexMembers.size(); i++)
	{
		vertexDesc[i] = DxDescriptorsConverter::GetInputElementDesc(vertexMembers[i]);
	}

	auto shaderBin = buffer->GetVsShaderBinary();
	ComPtr<ID3D11InputLayout> inputLayout;
	_device->CreateInputLayout(vertexDesc.get(), vertexMembers.size(), shaderBin->GetData(), shaderBin->GetSize(), inputLayout.GetAddressOf()) >> HrCheck::Instance();
	_context->IASetInputLayout(inputLayout.Get());

}

void Potator::Dx11GraphicsDevice::Draw(const MeshComponent& mesh)
{
	//_context->DrawIndexed(mesh.IndexCount, mesh.StartIndexLocation, 0);
	_context->Draw(3, 0);
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
		case ShaderType::Pixel:
		{
			ComPtr<ID3D11PixelShader> pixelShader;
			_device->CreatePixelShader(shader->GetData(), shader->GetSize(), nullptr, &pixelShader) >> HrCheck::Instance();
			_context->PSSetShader(pixelShader.Get(), nullptr, 0);
			break;
		}
		default:
			throw Exception("Unsupported shader type");
	}
}
