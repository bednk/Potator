#pragma once
#include "IShaderBinary.h"
#include <memory>

namespace Potator
{
	struct MaterialComponent
	{
		VertexShaderHandle VertexShader;
		std::shared_ptr<IShaderBinary> VsBinary;
		PixelShaderHandle PixelShader;
		InputLayoutHandle InputLayout;
	};
}