#pragma once

namespace Potator
{
	enum class PipelineStage
	{
		VertexShader,
		PixelShader
	};

	enum class PsConstantBufferSlots
	{
		MaterialDescriptor = 0,
		EnviromentLight = 1,
		PointLights = 2
	};

	enum class VsConstantBufferSlots
	{
		ModelTransform = 0,
		ViewProjTransform = 1
	};
}