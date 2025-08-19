#pragma once
#include "framework.h"
#include "IGraphicsDevice.h"
#include "Dx11GraphicsDevice.h"
#include "Exception.h"
#include <memory>

namespace Potator
{
	class DeviceFactory
	{
	public:
		static std::unique_ptr<IGraphicsDevice> GetDevice(GpuApi api, HWND windowHandle)
		{
			switch (api)
			{
			case GpuApi::Dx11:
				return std::make_unique<Dx11GraphicsDevice>(windowHandle);
			default:
				throw Exception("Not implemented API");
			}
		}
	};
}