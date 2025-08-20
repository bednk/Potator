#pragma once
#include <queue>
#include "ICommand.h"
#include <optional>

namespace Potator
{
	class CommandQueueComponent
	{
	public:
		CommandQueueComponent(Entity owner);
		void Enqueue(ICommand* command);
		Entity GetOwner() { return _owner; }
		std::optional<ICommand*> Dequeue();
	private:
		Entity _owner;
		std::queue<ICommand*> _queue;
	};
}