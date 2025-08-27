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
		LightsConfig = 1
	};

	enum class VsConstantBufferSlots
	{
		ModelTransform = 0,
		ViewProjTransform = 1
	};

	enum class PSStructuredBufferSlots
	{
		MaterialTexture = 0,
		PointLights = 1
	};
}