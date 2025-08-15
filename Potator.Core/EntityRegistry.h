#pragma once
#include "Entity.h"
#include "Export.h"

namespace Potator
{
	class EX EntityRegistry
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

