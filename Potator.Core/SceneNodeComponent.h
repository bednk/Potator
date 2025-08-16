#pragma once
#include "Entity.h"
#include <vector>

namespace Potator
{
	struct SceneNodeComponent
	{
		Entity This;
		Entity Parent;
		std::vector<Entity> Children;
	};
}

