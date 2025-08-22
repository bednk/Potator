#include "pch.h"
#include "ViewManager.h"
#include "EntityRegistry.h"
#include <iostream>

Potator::CameraComponent GetDefault()
{
	return
	{
		0.1f,
		1000.0f,
		1.5,
	};
}

Potator::ViewManager::ViewManager(ComponentStorage<TransformComponent>& transforms, ComponentStorage<CameraComponent>& cameras, SceneGraph& scene, IGraphicsDevice* device, float aspectRatio) :
	_transforms { transforms },
	_cameras { cameras },
	_scene { scene },
	_device { device },
	_active { NONE_ENTITY },
	_transformationBuffer{ Eigen::Matrix4f::Identity() },
	_proj{ Eigen::Matrix4f::Identity() },
	_aspectRatio { aspectRatio }

{
	Entity defaultCam = EntityRegistry::Instance().GetNew();
	CameraComponent camComponent = GetDefault();
	TransformComponent transComponent;
	transComponent.Local <<
		0.707107f,	-0.408248f, -0.577350f, 5.0f,
		0.0f,		0.816497f,	-0.577350f, 5.0f,
		0.707107f,	0.408248f,	0.577350f,	-5.0f,
		0.0f,		0.0f,		0.0f,		1.0f;
	Add(defaultCam, camComponent, transComponent);
	SetActive(defaultCam);

	_transformationBuffer.Update(transComponent.World);
	_transformationHandle = _device->Create(&_transformationBuffer);
	_device->Bind(&_transformationHandle, PipelineStage::VertexShader, 1);
}

void Potator::ViewManager::UpdateView()
{
	if (_active == NONE_ENTITY)
	{
		return;
	}

	auto& cameraWorld = _transforms[_active].World;
	Eigen::Matrix4f projView = _proj * GetViewTransform(cameraWorld);
	projView.transposeInPlace();
	
	_transformationBuffer.Update(projView);
	_device->Update(&_transformationBuffer, &_transformationHandle);
}

void Potator::ViewManager::Add(Entity cameraEntity, CameraComponent camera, TransformComponent transform)
{
	_scene.AddNode(cameraEntity, transform);
	_cameras.Store(cameraEntity, camera);
}

void Potator::ViewManager::SetActive(Entity camera)
{
	_active = camera;
	_proj = GetProjTransform(_cameras[_active], _aspectRatio);
	ViewChanged(camera);
}

Potator::Entity Potator::ViewManager::GetActive()
{
	return _active;
}

void Potator::ViewManager::OnWindowResized(unsigned int width, unsigned int height)
{
	_aspectRatio = width / (float)height;
	_proj = GetProjTransform(_cameras[_active], _aspectRatio);
}

Eigen::Matrix4f Potator::ViewManager::GetViewTransform(const Eigen::Matrix4f& cameraWorld)
{
	Eigen::Matrix4f view = cameraWorld.inverse();

	return view;
}

Eigen::Matrix4f Potator::ViewManager::GetProjTransform(const CameraComponent& camera, float aspectRatio)
{
	float h = 1.0f / std::tan(camera.FovY * 0.5f);
	float w = h / aspectRatio;

	Eigen::Matrix4f result = Eigen::Matrix4f::Zero();

	result(0, 0) = w;
	result(1, 1) = h;
	result(2, 2) = camera.ZFar / (camera.ZFar - camera.ZNear);
	result(2, 3) = -camera.ZNear * camera.ZFar / (camera.ZFar - camera.ZNear);
	result(3, 2) = 1.0f;

	return result;
}
