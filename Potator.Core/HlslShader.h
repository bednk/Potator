#pragma once
#include "leanWin.h"
#include <d3d11.h>
#include "IShaderBinary.h"
#include <wrl/client.h>
#include <string>

namespace Potator
{
	class HlslShader : public IShaderBinary
	{
	public:
		HlslShader(std::wstring name);
		const void* GetData() const override;
		UINT GetSize() const override;
	private:
		Microsoft::WRL::ComPtr<ID3DBlob> _blob;
	};
}

