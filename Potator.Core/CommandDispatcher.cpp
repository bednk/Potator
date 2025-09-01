#include "CommandDispatcher.h"

Potator::CommandDispatcher::CommandDispatcher(ComponentStorage<CommandQueueComponent>& commandQueues) :
	_commandQueues { commandQueues }
{
}

void Potator::CommandDispatcher::Enqueue(Entity entity, std::shared_ptr<ICommand> command)
{
	if (!_commandQueues.HasComponent(entity))
	{
		CommandQueueComponent component(entity);
		component.Enqueue(command);
		_commandQueues.Store(entity, component);
		return;
	}
	CommandQueueComponent& component = _commandQueues[entity];
	component.Enqueue(command);
}

void Potator::CommandDispatcher::Dispatch()
{
	std::vector<CommandQueueComponent>& queues = _commandQueues.GetComponents();
	for (size_t i = 0; i < queues.size(); i++)
	{
		CommandQueueComponent& queue = queues[i];
		Entity owner = queue.GetOwner();
		std::optional<std::shared_ptr<ICommand>> command;

		while ((command = queue.Dequeue()))
		{
			command.value()->Execute(owner);
		}
	}
}
