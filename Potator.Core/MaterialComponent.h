#pragma once
#include "IShaderBinary.h"

namespace Potator
{
	struct MaterialComponent
	{
		VertexShaderHandle VertexShader;
		IShaderBinary* VsBinary;
		PixelShaderHandle PixelShader;
		InputLayoutHandle InputLayout;
	};
}