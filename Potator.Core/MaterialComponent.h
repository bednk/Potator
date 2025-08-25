#pragma once
#include "IShaderBinary.h"
#include <memory>
#include <optional>
#include "MaterialDescriptor.h"

namespace Potator
{
	struct MaterialComponent
	{
		std::optional<ShaderResourceHandle> Texture;
		std::shared_ptr<IShaderBinary> VsBinary;
		VertexShaderHandle VertexShader = { ~0u };
		PixelShaderHandle PixelShader = { ~0u };
		InputLayoutHandle InputLayout = { ~0u };
		ConstantBufferHandle DescriptorHandle = { ~0u };
	};
}