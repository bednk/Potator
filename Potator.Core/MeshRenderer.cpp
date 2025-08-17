#include "pch.h"
#include "MeshRenderer.h"
#include "boost/bind/bind.hpp"


Potator::MeshRenderer::MeshRenderer(IGraphicsDevice* graphicsDevice, ComponentStorage<MeshComponent>& meshes, ComponentStorage<TransformComponent>& transforms) :
	_graphicsDevice{ graphicsDevice },
	_meshes{ meshes },
	_transforms{ transforms },
	_transformationBuffer { Eigen::Matrix4f::Identity()}
{
	_meshes.ComponentAdded.connect([this](Entity e, const MeshComponent& c) { OnMeshAdded(e, c); });
	_meshes.ComponentRemoved.connect([this](Entity e) { OnMeshRemoved(e); });
	_transforms.ComponentAdded.connect([this](Entity e, const TransformComponent& c) { OnTransformAdded(e, c); });
	_transforms.ComponentRemoved.connect([this](Entity e) { OnTransformRemoved(e); });
	_transformationHandle = _graphicsDevice->Create(&_transformationBuffer);
	_graphicsDevice->Bind(&_transformationHandle, PipelineStage::VertexShader, 0);
}

void Potator::MeshRenderer::Render()
{
	for (size_t i = 0; i < _drawableEntities.size(); i++)
	{
		Entity entity = _drawableEntities[i];
		_transformationBuffer.Update(_transforms[entity].World);
		_graphicsDevice->Update(&_transformationBuffer, &_transformationHandle);
		_graphicsDevice->Draw(&_meshes[i]);
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
