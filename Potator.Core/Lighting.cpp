#include "Lighting.h"
#include "ConstantBuffer.h"
#include "EntityRegistry.h"

struct EnviromentLights
{
	Potator::AmbientLightComponent Ambient;
	Potator::DirectlionalLightComponent Directional;
};

Potator::Lighting::Lighting(ComponentStorage<PointLightComponent>& lights, ComponentStorage<TransformComponent>& transforms, IGraphicsDevice* device) :
	_lights { lights },
	_transforms { transforms },
	_device { device }
{
	_lights.ComponentAdded.connect([this](Entity e, const PointLightComponent& c) { OnLightAdded(e, c); });
	_lights.ComponentRemoved.connect([this](Entity e) { RemoveLight(e); });
	_transforms.ComponentAdded.connect([this](Entity e, const TransformComponent& c) { OnTransformAdded(e, c); });
	_transforms.ComponentRemoved.connect([this](Entity e) { RemoveLight(e); });

	EnviromentLights envLights;
	envLights.Directional.Dicection.normalize();

	ConstantBuffer<EnviromentLights> cpuEnv(envLights);
	ConstantBuffer<PointLightsBuffer> cpuPoint(_pointLightsBuffer);
	_enviromentLightsHandle = _device->Create(&cpuEnv);
	_pointLightsHandle = _device->Create(&cpuPoint);
	_device->Bind(&_enviromentLightsHandle, PipelineStage::PixelShader, (UINT)PsConstantBufferSlots::EnviromentLight);
	_device->Bind(&_pointLightsHandle, PipelineStage::PixelShader, (UINT)PsConstantBufferSlots::PointLights);
}

void Potator::Lighting::Update()
{
	_pointLightsBuffer.Count = min(_lightEntities.size(), 16);
	for (size_t i = 0; i < _pointLightsBuffer.Count; i++)
	{
		Entity entity = _lightEntities[i];
		_pointLightsBuffer.Lights[i] = _lights[entity];
		TransformComponent& transform = _transforms[entity];
		_pointLightsBuffer.Lights[i].Position = transform.World.block<3, 1>(0, 3);
		// ToDo: pick 16 closest based on current view transform
	}
	ConstantBuffer<PointLightsBuffer> cpuPoint(_pointLightsBuffer);
	_device->Update(&cpuPoint, &_pointLightsHandle);
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
