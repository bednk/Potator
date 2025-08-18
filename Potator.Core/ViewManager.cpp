#include "pch.h"
#include "ViewManager.h"
#include <iostream>

Eigen::Matrix4f PerspectiveFovLH(float fovY, float aspect, float znear, float zfar)
{
	float yScale = 1.0f / tanf(fovY * 0.5f);
	float xScale = yScale / aspect;

	Eigen::Matrix4f proj = Eigen::Matrix4f::Zero();
	proj(0, 0) = xScale;
	proj(1, 1) = yScale;
	proj(2, 2) = zfar / (zfar - znear);
	proj(2, 3) = 1.0f;
	proj(3, 2) = (-znear * zfar) / (zfar - znear);

	return proj;
}

Potator::ViewManager::ViewManager(ComponentStorage<TransformComponent>& transforms, SceneGraph& scene, IGraphicsDevice* device) :
	_transforms { transforms },
	_scene { scene },
	_device { device },
	_active { NONE_ENTITY },
	_transformationBuffer{ Eigen::Matrix4f::Identity() },
	_proj{ Eigen::Matrix4f::Identity() }

{
	_transformationHandle = _device->Create(&_transformationBuffer);
	_device->Bind(&_transformationHandle, PipelineStage::VertexShader, 1);
	_proj = PerspectiveFovLH(1.5, 4 / 3.0f, 0.1f, 1000.0f);
}

void Potator::ViewManager::UpdateView()
{
	if (_active == NONE_ENTITY)
	{
		return;
	}

	auto& cameraWorld = _transforms[_active].World;
	Eigen::Matrix4f projView = _proj;// *GetViewTransform(cameraWorld);
	
	_transformationBuffer.Update(projView);
	_device->Update(&_transformationBuffer, &_transformationHandle);
}

void Potator::ViewManager::Add(Entity camera)
{
	TransformComponent initialTransform;
	_scene.AddNode(camera, initialTransform);
	_cameras.push_back(camera);

	if (_cameras.size() == 1)
	{
		SetActive(camera);
	}
}

void Potator::ViewManager::SetActive(Entity camera)
{
	_active = camera;
}

Eigen::Matrix4f Potator::ViewManager::GetViewTransform(const Eigen::Matrix4f& cameraWorld)
{
	Eigen::Matrix4f view = cameraWorld.inverse();

	return view;
}
