#pragma once
#include "ComponentStorage.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "IGraphicsDevice.h"
#include "ConstantBuffer.h"
#include "SceneGraph.h"
#include <boost/signals2.hpp>

namespace Potator
{
	class ViewManager
	{
	public:
		ViewManager(ComponentStorage<TransformComponent>& transforms, ComponentStorage<CameraComponent>& cameras, SceneGraph& scene, IGraphicsDevice* device);
		void UpdateView();
		void Add(Entity cameraEntity, CameraComponent camera, TransformComponent transform);
		void SetActive(Entity camera);
		Entity GetActive();
		boost::signals2::signal<void(Entity)> ViewChanged;
	private:
		Eigen::Matrix4f GetViewTransform(const Eigen::Matrix4f& cameraWorld);
		Eigen::Matrix4f GetProjTransform(const CameraComponent& camera);
		ComponentStorage<TransformComponent>& _transforms;
		ComponentStorage<CameraComponent>& _cameras;
		SceneGraph& _scene;
		IGraphicsDevice* _device;
		Entity _active;
		ConstantBufferHandle _transformationHandle;
		ConstantBuffer<Eigen::Matrix4f> _transformationBuffer;
		Eigen::Matrix4f _proj;
	};
}

