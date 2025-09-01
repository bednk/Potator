#include "pch.h"
#include "SceneGraph.h"
#include <queue>

Potator::SceneGraph::SceneGraph(ComponentStorage<TransformComponent>& transforms, ComponentStorage<SceneNodeComponent>& tree) :
	_transforms { transforms },
	_nodes { tree }
{
	TopoSort();
}

void Potator::SceneGraph::AddNode(Entity entity, TransformComponent& transform, Entity parent)
{
	_transforms.Store(entity, transform);
	_topologicalOrder.push_back(entity);

	SceneNodeComponent node{ entity, parent };
	_nodes.Store(entity, node);

	if (!_nodes.HasComponent(parent))
	{
		return;
	}

	SceneNodeComponent& parentNode = _nodes[parent];
	parentNode.Children.push_back(entity);
}

Potator::SceneNodeComponent& Potator::SceneGraph::GetNode(Entity entity)
{
	return _nodes[entity];
}

void Potator::SceneGraph::Update()
{
	for (size_t i = 0; i < _topologicalOrder.size(); i++)
	{
		Entity entity = _topologicalOrder[i];
		if (!_transforms.HasComponent(entity))
		{
			continue;
		}

		auto& transform = _transforms[entity];

		Entity parent = _nodes[entity].Parent;
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
	auto& nodes = _nodes.GetComponents();
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
			if (!_nodes.HasComponent(child))
			{
				continue;
			}

			queue.push(_nodes[child]);
		}
	}
}
