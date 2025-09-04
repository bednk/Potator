#pragma once
#include <string>

namespace Potator
{
	enum class GpuApi
	{
		Dx11,
		OpenGL
	};

	struct LaunchingParams
	{
		std::string Title = "Potator";
		unsigned int Width = 800;
		unsigned int Height = 600;
		GpuApi Api = GpuApi::Dx11;
		unsigned int FixedStepRate = 100;
	};
}