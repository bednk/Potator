#pragma once
#include "framework.h"
#include "IGraphicsDevice.h"
#include "Dx11GraphicsDevice.h"
#include "DxShaderCache.h"
#include "Exception.h"
#include "LaunchingParams.h"
#include "IShaderCache.h"
#include <memory>

namespace Potator
{
	class DeviceFactory
	{
	public:
		static std::unique_ptr<IGraphicsDevice> GetDevice(LaunchingParams settings, HWND windowHandle)
		{
			switch (settings.Api)
			{
			case GpuApi::Dx11:
				return std::make_unique<Dx11GraphicsDevice>(windowHandle, settings);
			default:
				throw Exception("Not implemented API");
			}
		}

		static std::unique_ptr<IShaderCache> GetShaderCache(GpuApi api, IGraphicsDevice* device)
		{
			switch (api)
			{
			case GpuApi::Dx11:
				return std::make_unique<DxShaderCache>(device);
			default:
				throw Exception("Not implemented API");
			}
		}
	};
}