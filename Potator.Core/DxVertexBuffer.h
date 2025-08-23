#pragma once
#include <wrl/client.h>
#include <d3d11.h>

namespace Potator
{
	struct DxVertexBuffer
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> Buffer;
		UINT Stride{ ~0u };
	};
}