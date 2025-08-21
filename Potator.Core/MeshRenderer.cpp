#include "pch.h"
#include "MeshRenderer.h"
#include "boost/bind/bind.hpp"
#include "MovementSystem.h"


Potator::MeshRenderer::MeshRenderer(IGraphicsDevice* graphicsDevice, ComponentStorage<MeshComponent>& meshes, ComponentStorage<TransformComponent>& transforms, ComponentStorage<MaterialComponent>& materials) :
	_graphicsDevice{ graphicsDevice },
	_meshes{ meshes },
	_transforms{ transforms },
	_materials { materials },
	_transformationBuffer { Eigen::Matrix4f::Identity()}
{
	_meshes.ComponentAdded.connect([this](Entity e, const MeshComponent& c) { OnMeshAdded(e, c); });
	_meshes.ComponentRemoved.connect([this](Entity e) { RemoveDrawable(e); });
	_transforms.ComponentAdded.connect([this](Entity e, const TransformComponent& c) { OnTransformAdded(e, c); });
	_transforms.ComponentRemoved.connect([this](Entity e) { RemoveDrawable(e); });
	_materials.ComponentAdded.connect([this](Entity e, const MaterialComponent& c) { OnMaterialAdded(e, c); });
	_materials.ComponentRemoved.connect([this](Entity e) { RemoveDrawable(e); });
	_transformationHandle = _graphicsDevice->Create(&_transformationBuffer);
	_graphicsDevice->Bind(&_transformationHandle, PipelineStage::VertexShader, 0);
}

void Potator::MeshRenderer::Render()
{
	for (size_t i = 0; i < _drawableEntities.size(); i++)
	{
		Entity entity = _drawableEntities[i];
		Eigen::Matrix4f trans = _transforms[entity].World.transpose();
		_transformationBuffer.Update(trans);
		_graphicsDevice->Update(&_transformationBuffer, &_transformationHandle);
		_graphicsDevice->Draw(&_meshes[entity], &_materials[entity]);
	}
}

void Potator::MeshRenderer::OnMeshAdded(Entity entity, const MeshComponent& component)
{
	if (!_transforms.HasComponent(entity) || !_materials.HasComponent(entity))
	{
		return;
	}

	_drawableEntities.push_back(entity);
}

void Potator::MeshRenderer::OnTransformAdded(Entity entity, const TransformComponent& component)
{
	if (!_meshes.HasComponent(entity) || !_materials.HasComponent(entity))
	{
		return;
	}

	_drawableEntities.push_back(entity);
}

void Potator::MeshRenderer::OnMaterialAdded(Entity entity, const MaterialComponent& component)
{
	if (!_meshes.HasComponent(entity) || !_transforms.HasComponent(entity))
	{
		return;
	}

	_drawableEntities.push_back(entity);
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
