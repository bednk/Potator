#pragma once
#include "IShaderBinary.h"
#include <memory>
#include <string>

namespace Potator
{
	class IShaderCache
	{
	public:
		virtual ~IShaderCache() = default;
		virtual std::shared_ptr<IShaderBinary> GetShaderBinary(const std::string& name) = 0;
		virtual VertexShaderHandle GetVertexShaderHandle(const std::string& name) = 0;
		virtual PixelShaderHandle GetPixelShaderHandle(const std::string& name) = 0;
	};
}