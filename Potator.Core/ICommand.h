#pragma once
#include "Entity.h"

namespace Potator
{
	class ICommand
	{
	public:
		virtual ~ICommand() = default;
		virtual void Execute(Entity entity) = 0;
	};
}
