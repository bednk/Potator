#pragma once
#include "ComponentStorage.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"
#include "TransformComponent.h"
#include "VelocityComponent.h"
#include "SceneNodeComponent.h"
#include "CommandQueueComponent.h"
#include "CameraComponent.h"
#include "PointLightComponent.h"
#include "ScriptComponent.h"

namespace Potator
{
	struct Components
	{
        Components(ComponentStorage<MeshComponent>& meshes,
            ComponentStorage<MaterialComponent>& materials,
            ComponentStorage<TransformComponent>& transforms,
            ComponentStorage<VelocityComponent>& movements,
            ComponentStorage<SceneNodeComponent>& nodes,
            ComponentStorage<CommandQueueComponent>& commandQueues,
            ComponentStorage<CameraComponent>& cameras,
            ComponentStorage<PointLightComponent>& pointLights,
            ComponentStorage<ScriptComponent>& scripts) :
                Meshes{ meshes },
                Materials{ materials },
                Transforms{ transforms },
                Movements{ movements },
                Nodes{ nodes },
                CommandQueues{ commandQueues },
                Cameras{ cameras },
                PointLights{ pointLights },
                Scripts{ scripts }
        {

        }
        ComponentStorage<MeshComponent>& Meshes;
        ComponentStorage<MaterialComponent>& Materials;
        ComponentStorage<TransformComponent>& Transforms;
        ComponentStorage<VelocityComponent>& Movements;
        ComponentStorage<SceneNodeComponent>& Nodes;
        ComponentStorage<CommandQueueComponent>& CommandQueues;
        ComponentStorage<CameraComponent>& Cameras;
        ComponentStorage<PointLightComponent>& PointLights;
        ComponentStorage<ScriptComponent>& Scripts;
	};
}