#include "pch.h"
#include "Dx11GraphicsDevice.h"
#include "HRCheck.h"
#include "DxDescriptorsConverter.h"


using namespace Microsoft::WRL;
using namespace Potator;

Potator::Dx11GraphicsDevice::Dx11GraphicsDevice(HWND windowHandle, LaunchingParams params)
{
	InitializeDevice(windowHandle);
	RecreateRenderTargeView();
	RecreateZBuffer(params.Width, params.Height);
	SetViewport(params.Width, params.Height);
	SetDefaultSampler();
	_context->OMSetRenderTargets(1, _targetView.GetAddressOf(), _depthStencilView.Get());
	_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // possibility to move this to mesh/material component
}

void Potator::Dx11GraphicsDevice::RecreateZBuffer(unsigned int w, unsigned int h)
{
	D3D11_DEPTH_STENCIL_DESC zBufferStateDesc = {};
	zBufferStateDesc.DepthEnable = true;
	zBufferStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	zBufferStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

	ComPtr<ID3D11DepthStencilState> zBufferState;
	_device->CreateDepthStencilState(&zBufferStateDesc, zBufferState.GetAddressOf()) >> HrCheck::Instance();
	_context->OMSetDepthStencilState(zBufferState.Get(), 1);

	D3D11_TEXTURE2D_DESC zBufferTexDesc = {};
	zBufferTexDesc.Width = w;
	zBufferTexDesc.Height = h;
	zBufferTexDesc.MipLevels = 1;
	zBufferTexDesc.ArraySize = 1;
	zBufferTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
	zBufferTexDesc.SampleDesc.Count = 1;
	zBufferTexDesc.SampleDesc.Quality = 0;
	zBufferTexDesc.Usage = D3D11_USAGE_DEFAULT;
	zBufferTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ComPtr<ID3D11Texture2D> zBufferTex;
	_device->CreateTexture2D(&zBufferTexDesc, nullptr, zBufferTex.GetAddressOf()) >> HrCheck::Instance();

	D3D11_DEPTH_STENCIL_VIEW_DESC zBufferViewDesc = {};
	zBufferViewDesc.Format = zBufferTexDesc.Format;
	zBufferViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	zBufferViewDesc.Texture2D.MipSlice = 0;

	_device->CreateDepthStencilView(zBufferTex.Get(), &zBufferViewDesc, _depthStencilView.GetAddressOf()) >> HrCheck::Instance();
}


void Potator::Dx11GraphicsDevice::SetDefaultSampler()
{
	// maybe in the future it will become a part of MaterialComponent
	ComPtr<ID3D11SamplerState> sampler;
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	_device->CreateSamplerState(&samplerDesc, sampler.GetAddressOf()) >> HrCheck::Instance();
	_context->PSSetSamplers(0, 1, sampler.GetAddressOf());
}

void Potator::Dx11GraphicsDevice::InitializeDevice(HWND windowHandle)
{
#ifdef _DEBUG
	UINT devFlags = D3D11_CREATE_DEVICE_DEBUG;
#else
	UINT devFlags = 0;
#endif

	// init device, context and swapchain
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
#pragma region swapChainDesc
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
#pragma endregion

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		devFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		_swapChain.GetAddressOf(),
		_device.GetAddressOf(),
		nullptr,
		_context.GetAddressOf()
	);

#ifdef _DEBUG
	Microsoft::WRL::ComPtr<ID3D11InfoQueue> infoQueue;
	_device->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)infoQueue.GetAddressOf()) >> HrCheck::Instance();
	infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_WARNING, true);
	infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
	infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
#endif
}

void Potator::Dx11GraphicsDevice::Clear(float r, float g, float b, float a)
{
	const float color[]{ r, g, b, a };
	_context->ClearRenderTargetView(_targetView.Get(), color);
	_context->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1, 0);
}

void Potator::Dx11GraphicsDevice::Draw(const MeshComponent* mesh, const MaterialComponent* material)
{
	if (material->Texture.has_value())
	{
		Bind(&material->Texture.value(), PipelineStage::PixelShader, (UINT)PSStructuredBufferSlots::MaterialTexture);
	}

	Bind(&mesh->VertexBuffer);
	Bind(&mesh->IndexBuffer);
	Bind(&material->InputLayout);
	Bind(&material->VertexShader);
	Bind(&material->PixelShader);
	Bind(&material->DescriptorHandle, PipelineStage::PixelShader, (UINT)PsConstantBufferSlots::MaterialDescriptor);
	
	_context->DrawIndexed(mesh->IndexCount, mesh->StartIndexLocation, 0);
}

void Potator::Dx11GraphicsDevice::Present()
{
	_swapChain->Present(1, 0);
}

void Potator::Dx11GraphicsDevice::OnWindowResized(unsigned int width, unsigned int height)
{
	_targetView->Release();
	_depthStencilView->Release();
	_swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	RecreateRenderTargeView();
	RecreateZBuffer(width, height);
	SetViewport(width, height);
	_context->OMSetRenderTargets(1, _targetView.GetAddressOf(), _depthStencilView.Get());
}

void Potator::Dx11GraphicsDevice::RecreateRenderTargeView()
{
	ComPtr<ID3D11Resource> backBuffer;
	_swapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _targetView.GetAddressOf()) >> HrCheck::Instance();
}

void Potator::Dx11GraphicsDevice::SetViewport(unsigned int w, unsigned int h)
{
	D3D11_VIEWPORT viewPort = {};
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.MaxDepth = 1;
	viewPort.MinDepth = 0;
	viewPort.Width = (float)w;
	viewPort.Height = (float)h;
	_context->RSSetViewports(1, &viewPort);
}

ShaderResourceHandle Potator::Dx11GraphicsDevice::Create2dTexture(const RgbaTextureContainer* source)
{
	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = source->Pixels.data();
	data.SysMemPitch = source->Width * 4;

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = source->Width;
	desc.Height = source->Height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	ComPtr<ID3D11Texture2D> tex;
	_device->CreateTexture2D(&desc, &data, tex.GetAddressOf()) >> HrCheck::Instance();
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	ComPtr<ID3D11ShaderResourceView>& srv = _shaderResources.emplace_back();
	_device->CreateShaderResourceView(tex.Get(), &srvDesc, srv.GetAddressOf()) >> HrCheck::Instance();

	return { _shaderResources.size() - 1 };
}

StructuredBufferHandle Potator::Dx11GraphicsDevice::CreateStructuredBuffer(const IConstantBuffer* buffer)
{
	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = buffer->GetData();

	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = buffer->GetSize();
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = buffer->GetStride();

	ComPtr<ID3D11Buffer>& structuredBuffer = _generalBuffers.emplace_back();
	ComPtr<ID3D11ShaderResourceView>& srv = _shaderResources.emplace_back();
	
	_device->CreateBuffer(&desc, &data, structuredBuffer.GetAddressOf()) >> HrCheck::Instance();
	
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = buffer->GetSize() / buffer->GetStride();

	
	_device->CreateShaderResourceView(structuredBuffer.Get(), &srvDesc, srv.GetAddressOf());

	return { _generalBuffers.size() - 1,  _shaderResources.size() - 1 };
}

void Potator::Dx11GraphicsDevice::Bind(const ShaderResourceHandle* resource, PipelineStage stage, UINT slot)
{
	ComPtr<ID3D11ShaderResourceView>& srv = _shaderResources[resource->Id];
	switch (stage)
	{
	case PipelineStage::VertexShader:
		_context->VSSetShaderResources(slot, 1, srv.GetAddressOf());
		return;
	case PipelineStage::PixelShader:
		_context->PSSetShaderResources(slot, 1, srv.GetAddressOf());
		return;
	default:
		throw std::invalid_argument("Unsupported stage");
	}
}

void Potator::Dx11GraphicsDevice::Bind(const VertexShaderHandle* shader)
{
	_context->VSSetShader(_vertexShaders[shader->Id].Get(), nullptr, 0);
}

void Potator::Dx11GraphicsDevice::Bind(const PixelShaderHandle* shader)
{
	_context->PSSetShader(_pixelShaders[shader->Id].Get(), nullptr, 0);
}

void Potator::Dx11GraphicsDevice::Bind(const InputLayoutHandle* inputLayout)
{
	_context->IASetInputLayout(_inputLayouts[inputLayout->Id].Get());
}

Potator::VertexShaderHandle Potator::Dx11GraphicsDevice::CreateVertexShader(const IShaderBinary* shaderBinary)
{
	auto& vertexShader = _vertexShaders.emplace_back();
	_device->CreateVertexShader(shaderBinary->GetData(), shaderBinary->GetSize(), nullptr, &vertexShader) >> HrCheck::Instance();
	return { _vertexShaders.size() - 1 };
}

Potator::PixelShaderHandle Potator::Dx11GraphicsDevice::CreatePixelShader(const IShaderBinary* shaderBinary)
{
	auto& pixelShader = _pixelShaders.emplace_back();
	_device->CreatePixelShader(shaderBinary->GetData(), shaderBinary->GetSize(), nullptr, &pixelShader) >> HrCheck::Instance();
	return { _pixelShaders.size() - 1 };
}

void Potator::Dx11GraphicsDevice::Update(const IConstantBuffer* data, const ConstantBufferHandle* gpuHandle)
{
	auto& buffer = _generalBuffers[gpuHandle->Id];
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	_context->Map(
		buffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource
	) >> HrCheck::Instance();

	memcpy(mappedResource.pData, data->GetData(), data->GetSize());
	_context->Unmap(buffer.Get(), 0);
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
	auto& vertexBuffer = _vertexBuffers.emplace_back();
	vertexBuffer.Stride = bufferDesc.StructureByteStride;
	_device->CreateBuffer(&bufferDesc, &data, vertexBuffer.Buffer.GetAddressOf()) >> HrCheck::Instance();

	return { _vertexBuffers.size() - 1 };
}

Potator::InputLayoutHandle Potator::Dx11GraphicsDevice::CreateInputLayout(const std::vector<VertexMemberDescriptor>& vertexMembers, const IShaderBinary* shaderBin)
{
	auto vertexDesc = std::make_unique<D3D11_INPUT_ELEMENT_DESC[]>(vertexMembers.size());
	for (size_t i = 0; i < vertexMembers.size(); i++)
	{
		vertexDesc[i] = DxDescriptorsConverter::GetInputElementDesc(vertexMembers[i]);
	}
	auto& inputLayout = _inputLayouts.emplace_back();
	_device->CreateInputLayout(vertexDesc.get(), (UINT)vertexMembers.size(), shaderBin->GetData(), shaderBin->GetSize(), inputLayout.GetAddressOf()) >> HrCheck::Instance();
	return { _inputLayouts.size() - 1};
}

Potator::IndexBufferHandle Potator::Dx11GraphicsDevice::Create(const IndexBuffer* buffer)
{
	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = buffer->GetData();

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.ByteWidth = buffer->GetSize();
	bufferDesc.StructureByteStride = buffer->GetStride();

	ComPtr<ID3D11Buffer>& indexBuffer = _generalBuffers.emplace_back();
	_device->CreateBuffer(&bufferDesc, &data, &indexBuffer) >> HrCheck::Instance();
	return { _generalBuffers.size() - 1 };
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

	ComPtr<ID3D11Buffer>& constantBuffer = _generalBuffers.emplace_back();
	_device->CreateBuffer(&bufferDesc, &data, &constantBuffer) >> HrCheck::Instance();
	return { _generalBuffers.size() - 1 };
}

void Potator::Dx11GraphicsDevice::Bind(const VertexBufferHandle* buffer)
{
	auto& vxBuffer = _vertexBuffers[buffer->Id];
	UINT _ = 0;
	_context->IASetVertexBuffers(0, 1, vxBuffer.Buffer.GetAddressOf(), &vxBuffer.Stride, &_);
}

void Potator::Dx11GraphicsDevice::Bind(const IndexBufferHandle* buffer)
{
	auto& idxBuffer = _generalBuffers[buffer->Id];
	_context->IASetIndexBuffer(idxBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}

void Potator::Dx11GraphicsDevice::Bind(const ConstantBufferHandle* buffer, PipelineStage stage, UINT slot)
{
	auto& cBuffer = _generalBuffers[buffer->Id];
	switch (stage)
	{
	case PipelineStage::VertexShader:
		_context->VSSetConstantBuffers(slot, 1, cBuffer.GetAddressOf());
		break;
	case PipelineStage::PixelShader:
		_context->PSSetConstantBuffers(slot, 1, cBuffer.GetAddressOf());
		break;
	default:
		throw std::invalid_argument("Unsupported pipeline stage");
	}
}