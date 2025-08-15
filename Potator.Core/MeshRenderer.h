#pragma once
#include "IGraphicsDevice.h"
#include "Entity.h"
#include "ComponentStorage.h"
#include "MeshComponent.h"
#include "TransformComponent.h"



namespace Potator
{
	class MeshRenderer
	{
	public:
		MeshRenderer(IGraphicsDevice* graphicsDevice, ComponentStorage<MeshComponent>& meshes, ComponentStorage<TransformComponent>& transforms);
		void Render();
	private:
		void OnMeshAdded(Entity entity, const MeshComponent& component);
		void OnMeshRemoved(Entity entity);
		void OnTransformAdded(Entity entity, const TransformComponent& component);
		void OnTransformRemoved(Entity entity);
		void RemoveDrawable(Entity entity);
		IGraphicsDevice* _graphicsDevice;
		ComponentStorage<MeshComponent>& _meshes;
		ComponentStorage<TransformComponent> &_transforms;
		std::vector<Entity> _drawableEntities;
	};
}

