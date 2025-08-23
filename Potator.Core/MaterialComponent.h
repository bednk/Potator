#pragma once
#include "IShaderBinary.h"
#include <memory>

namespace Potator
{
	struct MaterialComponent
	{
		VertexShaderHandle VertexShader = { ~0u };
		std::shared_ptr<IShaderBinary> VsBinary;
		PixelShaderHandle PixelShader = { ~0u };
		InputLayoutHandle InputLayout = { ~0u };
		ShaderResourceHandle Texture = { ~0u };
	};
}