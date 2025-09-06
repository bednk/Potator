#pragma once
#include "ComponentStorage.h"
#include "PointLightComponent.h"
#include "AmbientLightComponent.h"
#include "DirectlionalLightComponent.h"
#include "TransformComponent.h"
#include "IGraphicsDevice.h"
#include "ConstantBuffer.h"
#include "StructuredBuffer.h"


namespace Potator
{
	struct LightsConfig
	{
		Potator::AmbientLightComponent Ambient;
		Potator::DirectlionalLightComponent Directional;
		unsigned int PointLightsCount = 0;
		unsigned int pad[3] = {};
	};

	class Lighting
	{
	public:
		Lighting(ComponentStorage<PointLightComponent>& lights,
			ComponentStorage<TransformComponent>& transforms,
			std::shared_ptr<IGraphicsDevice> device);
		LightsConfig& GetConfig();
		void Update();
	private:
		void OnTransformAdded(Entity entity, const TransformComponent& component);
		void OnLightAdded(Entity entity, const PointLightComponent& component);
		void RemoveLight(Entity entity);
		LightsConfig _lightsConfig;
		ConstantBuffer<LightsConfig> _lcBuffer;
		ComponentStorage<PointLightComponent>& _lights;
		ComponentStorage<TransformComponent>& _transforms;
		ConstantBufferHandle _lightsConfigHandle;
		StructuredBufferHandle _pointLightsHandle;
		std::shared_ptr<IGraphicsDevice> _device;
		std::vector<Entity> _lightEntities;
		static constexpr size_t _maxPointLights = 16;
		PointLightComponent _pointLights[_maxPointLights];
		StructuredBuffer<PointLightComponent, _maxPointLights> _cpuPointLightsBuffer;
	};
}