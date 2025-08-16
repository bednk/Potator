#pragma once
#include <memory>
#include "ComponentStorage.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "SceneNodeComponent.h"
#include "EntityRegistry.h"
#include "IGraphicsDevice.h"

namespace Potator
{
	class Engine
	{
	public:
		Engine();
		void Run();
		ComponentStorage<MeshComponent>& GetMeshes();
		ComponentStorage<TransformComponent>& GetTransforms();
		ComponentStorage<SceneNodeComponent>& GetSceneGraph();
		EntityRegistry& GetEntityRegistry() const;
		IGraphicsDevice* GetGraphicsDevice() const;
		~Engine();
	private:
		struct Impl;
		std::unique_ptr<Impl> _impl;
	};
}
