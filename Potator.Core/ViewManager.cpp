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

Potator::ViewManager::ViewManager(ComponentStorage<TransformComponent>& transforms, ComponentStorage<CameraComponent>& cameras, SceneGraph& scene, std::shared_ptr<IGraphicsDevice> device, LaunchingParams launchingParams) :
	_transforms { transforms },
	_cameras { cameras },
	_scene { scene },
	_device { device },
	_active { NONE_ENTITY },
	_projViewBuffer{ Eigen::Matrix4f::Identity() },
	_proj{ Eigen::Matrix4f::Identity() },
	_aspectRatio { (float)launchingParams.Width / launchingParams.Height },
	_worldPosBuffer { Eigen::Vector4f::Zero() }

{
	Entity defaultCam = EntityRegistry::Instance().GetNew();
	CameraComponent camComponent = GetDefault();
	TransformComponent transComponent;
	transComponent.Local <<
		0.707107f,	-0.408248f, -0.577350f, 100.0f,
		0.0f,		0.816497f,	-0.577350f, 100.0f,
		0.707107f,	0.408248f,	0.577350f,	-100.0f,
		0.0f,		0.0f,		0.0f,		1.0f;
	//transComponent.Local(2, 3) = -200;
	Add(defaultCam, camComponent, transComponent);
	SetActive(defaultCam);

	Eigen::Matrix4f projView = _proj * GetViewTransform(transComponent.World);
	_projViewBuffer.Update(transComponent.World);
	_transformationHandle = _device->Create(&_projViewBuffer);
	_device->Bind(&_transformationHandle, PipelineStage::VertexShader, (UINT)VsConstantBufferSlots::ViewProjTransform);

	Eigen::Vector4f cameraWorldPosition = transComponent.World.col(3);
	_worldPosBuffer.Update(cameraWorldPosition);
	_worldPosHandle = _device->Create(&_worldPosBuffer);
	_device->Bind(&_worldPosHandle, PipelineStage::PixelShader, (UINT)PsConstantBufferSlots::CameraWorld);
}

void Potator::ViewManager::Update()
{
	if (_active == NONE_ENTITY)
	{
		return;
	}

	Eigen::Matrix4f& cameraWorldTransform = _transforms[_active].World;
	Eigen::Matrix4f projView = _proj * GetViewTransform(cameraWorldTransform);
	projView.transposeInPlace();
	
	_projViewBuffer.Update(projView);
	_device->Update(&_projViewBuffer, &_transformationHandle);

	Eigen::Vector4f cameraWorldPosition = cameraWorldTransform.col(3);
	_worldPosBuffer.Update(cameraWorldPosition);
	_device->Update(&_worldPosBuffer, &_worldPosHandle);
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
