#pragma once
#include "IShaderCache.h"
#include <unordered_map>
#include "IGraphicsDevice.h"



namespace Potator
{
	class DxShaderCache : public IShaderCache
	{
	public:
		DxShaderCache(IGraphicsDevice* device);
		std::shared_ptr<IShaderBinary> GetShaderBinary(const std::string& name);
		VertexShaderHandle GetVertexShaderHandle(const std::string& name);
		PixelShaderHandle GetPixelShaderHandle(const std::string& name);
	private:

		IGraphicsDevice* _device;
		std::unordered_map<std::string, std::shared_ptr<IShaderBinary>> _binaries;
		std::unordered_map<std::string, VertexShaderHandle> _vertexShaders;
		std::unordered_map<std::string, PixelShaderHandle> _pixelShaders;
	};
}