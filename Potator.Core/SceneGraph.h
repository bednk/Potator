#pragma once
#include "Entity.h"
#include "ComponentStorage.h"
#include "TransformComponent.h"
#include "SceneNodeComponent.h"

namespace Potator
{
	class SceneGraph
	{
	public:
		SceneGraph(ComponentStorage<TransformComponent>& transforms, ComponentStorage<SceneNodeComponent>& tree);
		void AddNode(Entity entity, TransformComponent& transform, Entity parent = NONE_ENTITY);
		SceneNodeComponent& GetNode(Entity entity);
		void UpdateTransforms();
	private:
		void TopoSort();
		ComponentStorage<TransformComponent>& _transforms;
		ComponentStorage<SceneNodeComponent>& _tree;
		std::vector<Entity> _topologicalOrder;
	};
}

