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

void Potator::Dx11GraphicsDevice::Draw(const MeshComponent* mesh)
{
	Bind(&mesh->VertexBuffer);
	Bind(&mesh->IndexBuffer);
	_context->DrawIndexed(mesh->IndexCount, mesh->StartIndexLocation, mesh->VertexOffset);
}

void Potator::Dx11GraphicsDevice::Present()
{
	_swapChain->Present(1, 0);
}


Potator::VertexBufferHandle Potator::Dx11GraphicsDevice::Create(const IVertexBuffer* buffer)
{
	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = buffer->GetData();

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.ByteWidth = buffer->GetSize();
	bufferDesc.StructureByteStride = buffer->GetStride();

	auto vertexMembers = buffer->GetVertexLayout();
	auto vertexDesc = std::make_unique<D3D11_INPUT_ELEMENT_DESC[]>(vertexMembers.size());
	for (size_t i = 0; i < vertexMembers.size(); i++)
	{
		vertexDesc[i] = DxDescriptorsConverter::GetInputElementDesc(vertexMembers[i]);
	}
	auto shaderBin = buffer->GetVsShaderBinary();

	auto& bufferCache = _vertexBuffers.emplace_back();
	bufferCache.Stride = bufferDesc.StructureByteStride;
	_device->CreateBuffer(&bufferDesc, &data, bufferCache.Buffer.GetAddressOf()) >> HrCheck::Instance();
	_device->CreateInputLayout(vertexDesc.get(), vertexMembers.size(), shaderBin->GetData(), shaderBin->GetSize(), bufferCache.InputLayout.GetAddressOf()) >> HrCheck::Instance();

	return { _vertexBuffers.size() - 1 };
}

Potator::IndexBufferHandle Potator::Dx11GraphicsDevice::Create(const IndexBuffer* buffer)
{
	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = buffer->GetData();

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.ByteWidth = buffer->GetSize();
	bufferDesc.StructureByteStride = buffer->GetStride();

	ComPtr<ID3D11Buffer>& indexBuffer = _indexBuffers.emplace_back();
	_device->CreateBuffer(&bufferDesc, &data, &indexBuffer) >> HrCheck::Instance();
	return { _indexBuffers.size() - 1 };
}

Potator::ConstantBufferHandle Potator::Dx11GraphicsDevice::Create(const IConstantBuffer* buffer)
{
	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = buffer->GetData();

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.ByteWidth = buffer->GetSize();
	bufferDesc.StructureByteStride = buffer->GetStride();

	ComPtr<ID3D11Buffer>& constantBuffer = _constantBuffers.emplace_back();
	_device->CreateBuffer(&bufferDesc, &data, &constantBuffer) >> HrCheck::Instance();
	return { _constantBuffers.size() - 1 };
}

void Potator::Dx11GraphicsDevice::Bind(const VertexBufferHandle* buffer)
{
	auto& vxBuffer = _vertexBuffers[buffer->Id];
	UINT _ = 0;
	_context->IASetVertexBuffers(0, 1, vxBuffer.Buffer.GetAddressOf(), &vxBuffer.Stride, &_);
	_context->IASetInputLayout(vxBuffer.InputLayout.Get());
}

void Potator::Dx11GraphicsDevice::Bind(const IndexBufferHandle* buffer)
{
	auto& idxBuffer = _indexBuffers[buffer->Id];
	_context->IASetIndexBuffer(idxBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}

void Potator::Dx11GraphicsDevice::Bind(const ConstantBufferHandle* buffer, PipelineStage stage, UINT slot)
{
	auto& cBuffer = _constantBuffers[buffer->Id];
	switch (stage)
	{
	case PipelineStage::VertexShader:
		_context->VSSetConstantBuffers(slot, 1, cBuffer.GetAddressOf());
		break;
	case PipelineStage::PixelShader:
		_context->PSSetConstantBuffers(slot, 1, cBuffer.GetAddressOf());
		break;
	default:
		throw Exception("Unsupported pipeline stage");
	}
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