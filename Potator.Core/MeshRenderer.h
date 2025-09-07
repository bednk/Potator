#pragma once
#include "IGraphicsDevice.h"
#include "Entity.h"
#include "ComponentStorage.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "ConstantBuffer.h"
#include "MaterialComponent.h"


namespace Potator
{
	class MeshRenderer
	{
	public:
		MeshRenderer(IGraphicsDevice& graphicsDevice,
			ComponentStorage<MeshComponent>& meshes,
			ComponentStorage<TransformComponent>& transforms,
			ComponentStorage<MaterialComponent>& materials);
		void Render();
	private:
		void OnMeshAdded(Entity entity, const MeshComponent& component);
		void OnTransformAdded(Entity entity, const TransformComponent& component);
		void OnMaterialAdded(Entity entity, const MaterialComponent& component);
		void RemoveDrawable(Entity entity);
		IGraphicsDevice& _graphicsDevice;
		ComponentStorage<MeshComponent>& _meshes;
		ComponentStorage<TransformComponent>& _transforms;
		ComponentStorage<MaterialComponent>& _materials;
		std::vector<Entity> _drawableEntities;
		ConstantBufferHandle _transformationHandle;
		ConstantBuffer<Eigen::Matrix4f> _transformationBuffer;
	};
}

