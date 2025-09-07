#pragma once
#include "ComponentStorage.h"
#include "ScriptComponent.h"
#include <sol/sol.hpp>
#include "MovementApi.h"

namespace Potator
{
	class ScriptingSystem
	{
	public:
		ScriptingSystem(CommandDispatcher& commandDispatcher,
			ComponentStorage<ScriptComponent>& scripts,
			ComponentStorage<VelocityComponent>& movements,
			ComponentStorage<TransformComponent>& transforms);
		void Update();
		sol::state& GetLuaState();
	private:
		void OnScriptAdded(Entity entity, const ScriptComponent& component);
		void RemoveScripted(Entity entity);
		ComponentStorage<ScriptComponent>& _scripts;
		sol::state _lua;
		MovementApi _movementApi;
		std::vector<Entity> _scripted;
		
	};
}