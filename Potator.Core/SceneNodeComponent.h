#pragma once
#include "Entity.h"
#include "Export.h"

namespace Potator
{
	struct EX SceneNodeComponent
	{
		Entity This;
		Entity Parent;
		std::vector<Entity> Children;
	};
}

