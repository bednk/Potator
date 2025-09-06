#include "Lighting.h"
#include "ConstantBuffer.h"
#include "EntityRegistry.h"
#include <array>

using namespace Potator;

Potator::Lighting::Lighting(ComponentStorage<PointLightComponent>& lights, ComponentStorage<TransformComponent>& transforms, std::shared_ptr<IGraphicsDevice> device) :
	_lights { lights },
	_transforms { transforms },
	_device { device },
	_lcBuffer { _lightsConfig },
	_cpuPointLightsBuffer { _pointLights }
{
	_lights.ComponentAdded.connect([this](Entity e, const PointLightComponent& c) { OnLightAdded(e, c); });
	_lights.ComponentRemoved.connect([this](Entity e) { RemoveLight(e); });
	_transforms.ComponentAdded.connect([this](Entity e, const TransformComponent& c) { OnTransformAdded(e, c); });
	_transforms.ComponentRemoved.connect([this](Entity e) { RemoveLight(e); });


	_lightsConfigHandle = _device->Create(&_lcBuffer);
	_device->Bind(&_lightsConfigHandle, PipelineStage::PixelShader, (UINT)PsConstantBufferSlots::LightsConfig);

	_pointLightsHandle = _device->CreateStructuredBuffer(&_cpuPointLightsBuffer);
	_device->Bind(&_pointLightsHandle.View, PipelineStage::PixelShader, (UINT)PSStructuredBufferSlots::PointLights);
	LightsConfig lc;
	Potator::ConstantBuffer<LightsConfig> lcBuffer(lc);
}

LightsConfig& Potator::Lighting::GetConfig()
{
	return _lightsConfig;
}

void Potator::Lighting::Update()
{
	_lightsConfig.PointLightsCount = min((unsigned int)_lightEntities.size(), _maxPointLights);
	_lcBuffer.Update(_lightsConfig);
	_device->Update(&_lcBuffer, &_lightsConfigHandle);

	for (size_t i = 0; i < _lightsConfig.PointLightsCount; i++)
	{
		Entity entity = _lightEntities[i];
		_pointLights[i] = _lights[entity];
		TransformComponent& transform = _transforms[entity];
		_pointLights[i].Position = transform.World.block<3, 1>(0, 3);
		// ToDo: pick 16 closest based on current view transform
	}

	_cpuPointLightsBuffer.Update(_pointLights);
	_device->Update(&_cpuPointLightsBuffer, &_pointLightsHandle.Buffer);
}

void Potator::Lighting::OnLightAdded(Entity entity, const PointLightComponent& component)
{
	if (!_transforms.HasComponent(entity))
	{
		return;
	}

	_lightEntities.push_back(entity);
}

void Potator::Lighting::RemoveLight(Entity entity)
{
	auto idx = std::find(_lightEntities.begin(), _lightEntities.end(), entity);
	if (idx == _lightEntities.end())
	{
		return;
	}

	_lightEntities.erase(idx);
}

void Potator::Lighting::OnTransformAdded(Entity entity, const TransformComponent& component)
{
	if (!_lights.HasComponent(entity))
	{
		return;
	}

	_lightEntities.push_back(entity);
}
