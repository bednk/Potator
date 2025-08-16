#pragma once
#include "Entity.h"

namespace Potator
{
	class EntityRegistry
	{
	public:
		Entity GetNew()
		{
			Entity entity = _nextEntity++;
			return entity;
		}
	private:
		Entity _nextEntity = 0;
	};
}

