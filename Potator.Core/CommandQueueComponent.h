#pragma once
#include <queue>
#include "ICommand.h"
#include <optional>
#include <memory>

namespace Potator
{
	class CommandQueueComponent
	{
	public:
		CommandQueueComponent(Entity owner);
		void Enqueue(std::shared_ptr<ICommand> command);
		Entity GetOwner() const { return _owner; }
		std::optional<std::shared_ptr<ICommand>> Dequeue();
	private:
		Entity _owner;
		std::queue<std::shared_ptr<ICommand>> _queue;
	};
}