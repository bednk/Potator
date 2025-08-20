#pragma once
#include "ComponentStorage.h"
#include "TransformComponent.h"
#include "IGraphicsDevice.h"
#include "ConstantBuffer.h"
#include "SceneGraph.h"
#include <boost/signals2.hpp>

namespace Potator
{
	class ViewManager
	{
	public:
		ViewManager(ComponentStorage<TransformComponent>& transforms, SceneGraph& scene, IGraphicsDevice* device);
		void UpdateView();
		void Add(Entity camera);
		void SetActive(Entity camera);
		boost::signals2::signal<void(Entity)> ViewChanged;
	private:
		Eigen::Matrix4f GetViewTransform(const Eigen::Matrix4f& cameraWorld);
		ComponentStorage<TransformComponent>& _transforms;
		SceneGraph& _scene;
		IGraphicsDevice* _device;
		Entity _active;
		ConstantBufferHandle _transformationHandle;
		ConstantBuffer<Eigen::Matrix4f> _transformationBuffer;
		std::vector<Entity> _cameras;
		Eigen::Matrix4f _proj;
	};
}

