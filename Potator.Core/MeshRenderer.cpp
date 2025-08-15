#include "pch.h"
#include "MeshRenderer.h"
#include "boost/bind/bind.hpp"


Potator::MeshRenderer::MeshRenderer(IGraphicsDevice* graphicsDevice, ComponentStorage<MeshComponent>& meshes, ComponentStorage<TransformComponent>& transforms) :
	_graphicsDevice{ graphicsDevice },
	_meshes{ meshes },
	_transforms{ transforms }
{
	_meshes.ComponentAdded.connect([this](Entity e, const MeshComponent& c) { OnMeshAdded(e, c); });
	_meshes.ComponentRemoved.connect([this](Entity e) { OnMeshRemoved(e); });
	_transforms.ComponentAdded.connect([this](Entity e, const TransformComponent& c) { OnTransformAdded(e, c); });
	_transforms.ComponentRemoved.connect([this](Entity e) { OnTransformRemoved(e); });
}

void Potator::MeshRenderer::Render()
{
	auto& meshes = _meshes.GetComponents();
	for (size_t i = 0; i < meshes.size(); i++)
	{
		_graphicsDevice->Draw(meshes[i]);
	}
}

void Potator::MeshRenderer::OnMeshAdded(Entity entity, const MeshComponent& component)
{
	if (!_transforms.HasComponent(entity))
	{
		return;
	}

	_drawableEntities.push_back(entity);
}

void Potator::MeshRenderer::OnMeshRemoved(Entity entity)
{
	RemoveDrawable(entity);
}

void Potator::MeshRenderer::OnTransformAdded(Entity entity, const TransformComponent& component)
{
	if (!_meshes.HasComponent(entity))
	{
		return;
	}

	_drawableEntities.push_back(entity);
}

void Potator::MeshRenderer::OnTransformRemoved(Entity entity)
{
	RemoveDrawable(entity);
}

void Potator::MeshRenderer::RemoveDrawable(Entity entity)
{
	auto idx = std::find(_drawableEntities.begin(), _drawableEntities.end(), entity);
	if (idx == _drawableEntities.end())
	{
		return;
	}

	_drawableEntities.erase(idx);
}
