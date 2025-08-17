#pragma once
#include <memory>
#include "ComponentStorage.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "SceneNodeComponent.h"
#include "EntityRegistry.h"
#include "IGraphicsDevice.h"
#include "SceneGraph.h"

namespace Potator
{
	class Engine
	{
	public:
		Engine();
		void Run();
		EntityRegistry& GetEntityRegistry() const;
		ComponentStorage<MeshComponent>& GetMeshes();
		SceneGraph& GetSceneGraph() const;
		IGraphicsDevice* GetGraphicsDevice() const;
		~Engine();
	private:
		struct Impl;
		std::unique_ptr<Impl> _impl;
	};
}
