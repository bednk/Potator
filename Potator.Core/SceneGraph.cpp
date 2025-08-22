#include "pch.h"
#include "SceneGraph.h"
#include <queue>

Potator::SceneGraph::SceneGraph(ComponentStorage<TransformComponent>& transforms, ComponentStorage<SceneNodeComponent>& tree) :
	_transforms { transforms },
	_tree { tree }
{
	TopoSort();
}

void Potator::SceneGraph::AddNode(Entity entity, TransformComponent& transform, Entity parent)
{
	_transforms.Store(entity, transform);
	_topologicalOrder.push_back(entity);

	SceneNodeComponent node{ entity, parent };
	_tree.Store(entity, node);

	if (!_tree.HasComponent(parent))
	{
		return;
	}

	SceneNodeComponent& parentNode = _tree[parent];
	parentNode.Children.push_back(entity);
}

Potator::SceneNodeComponent& Potator::SceneGraph::GetNode(Entity entity)
{
	return _tree[entity];
}

void Potator::SceneGraph::UpdateTransforms()
{
	for (size_t i = 0; i < _topologicalOrder.size(); i++)
	{
		Entity entity = _topologicalOrder[i];
		if (!_transforms.HasComponent(entity))
		{
			continue;
		}

		auto& transform = _transforms[entity];

		Entity parent = _tree[entity].Parent;
		if (!_transforms.HasComponent(parent))
		{
			transform.World = transform.Local;
			continue;
		}

		auto& parentTransform = _transforms[parent];
		transform.World = parentTransform.World * transform.Local;
	}
}

void Potator::SceneGraph::TopoSort()
{
	std::queue<SceneNodeComponent> queue;
	auto& nodes = _tree.GetComponents();
	for (size_t i = 0; i < nodes.size(); i++)
	{
		auto& node = nodes[i];
		if (node.Parent != NONE_ENTITY)
		{
			continue;
		}

		queue.push(node);
	}

	while (!queue.empty())
	{
		SceneNodeComponent next = queue.front();
		queue.pop();

		_topologicalOrder.push_back(next.This);
		for (size_t i = 0; i < next.Children.size(); i++)
		{
			Entity child = next.Children[i];
			if (!_tree.HasComponent(child))
			{
				continue;
			}

			queue.push(_tree[child]);
		}
	}
}
