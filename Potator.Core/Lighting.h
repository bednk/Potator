#pragma once
#include "ComponentStorage.h"
#include "PointLightComponent.h"
#include "AmbientLightComponent.h"
#include "DirectlionalLightComponent.h"
#include "TransformComponent.h"
#include "IGraphicsDevice.h"
#include "PointLightsBuffer.h"

namespace Potator
{
	class Lighting
	{
	public:
		Lighting(ComponentStorage<PointLightComponent>& lights, ComponentStorage<TransformComponent>& transforms, IGraphicsDevice* device);
		void Update();
	private:
		void OnTransformAdded(Entity entity, const TransformComponent& component);
		void OnLightAdded(Entity entity, const PointLightComponent& component);
		void RemoveLight(Entity entity);
		ComponentStorage<PointLightComponent>& _lights;
		ComponentStorage<TransformComponent>& _transforms;
		ConstantBufferHandle _enviromentLightsHandle;
		ConstantBufferHandle _pointLightsHandle;
		PointLightsBuffer _pointLightsBuffer;
		IGraphicsDevice* _device;
		std::vector<Entity> _lightEntities;
	};
}