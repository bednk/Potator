#include "ScriptingSystem.h"

using namespace Potator;

Potator::ScriptingSystem::ScriptingSystem(CommandDispatcher& commandDispatcher, ComponentStorage<ScriptComponent>& scripts, ComponentStorage<VelocityComponent>& movements, ComponentStorage<TransformComponent>& transforms)
	: _scripts { scripts },
	_lua {},
	_movementApi { commandDispatcher, movements, transforms, _lua }
{
	_lua.open_libraries(sol::lib::base);
	_scripts.ComponentAdded.connect([this](Entity e, const ScriptComponent& c) { OnScriptAdded(e, c); });
	_scripts.ComponentRemoved.connect([this](Entity e) { RemoveScripted(e); });
}


void ScriptingSystem::Update()
{
	for (size_t i = 0; i < _scripted.size(); i++)
	{
		Entity entity = _scripted[i];
		ScriptComponent& script = _scripts[entity];

		_lua["entity"] = entity;

		auto result = _lua.safe_script(script.Script, sol::script_pass_on_error);
		if (!result.valid())
		{
			sol::error err = result;
			std::cerr << "Lua error: " << err.what() << "\n";
		}
	}
}

sol::state& Potator::ScriptingSystem::GetLuaState()
{
	return _lua;
}

void Potator::ScriptingSystem::OnScriptAdded(Entity entity, const ScriptComponent& component)
{
	_scripted.push_back(entity);
}

void Potator::ScriptingSystem::RemoveScripted(Entity entity)
{
	auto idx = std::find(_scripted.begin(), _scripted.end(), entity);
	if (idx == _scripted.end())
	{
		return;
	}

	_scripted.erase(idx);
}
