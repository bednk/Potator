#pragma once
#include "ComponentStorage.h"
#include "CommandQueueComponent.h"

namespace Potator
{
	class CommandDispatcher
	{
	public:
		CommandDispatcher(ComponentStorage<CommandQueueComponent>& commandQueues);
		void Enqueue(Entity entity, std::shared_ptr<ICommand> command);
		void Dispatch();
	private:
		ComponentStorage<CommandQueueComponent>& _commandQueues;
	};
}

