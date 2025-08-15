#pragma once
#include "framework.h"
#include <d3d11.h>
#include "IShaderBinary.h"
#include <wrl/client.h>
#include <string>

namespace Potator
{
	class HlslShader : public IShaderBinary
	{
	public:
		HlslShader(std::wstring name, ShaderType type);
		const void* GetData() const override;
		UINT GetSize() const override;
		ShaderType GetType() const override;
	private:
		Microsoft::WRL::ComPtr<ID3DBlob> _blob;
		ShaderType _type;
	};
}

