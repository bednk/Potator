#pragma once
#include "Entity.h"

namespace Potator
{
	class EntityRegistry
	{
	public:
		EntityRegistry() = default;
		EntityRegistry(const EntityRegistry&) = delete;
		EntityRegistry& operator=(const EntityRegistry&) = delete;

		static EntityRegistry& Instance()
		{
			static EntityRegistry instance;
			return instance;
		}

		Entity GetNew()
		{
			Entity entity = _nextEntity++;
			return entity;
		}
	private:
		Entity _nextEntity = 0;
	};
}

