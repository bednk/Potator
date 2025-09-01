#include "CommandQueueComponent.h"
using namespace Potator;
Potator::CommandQueueComponent::CommandQueueComponent(Entity owner) :
	_owner { owner }
{
}

void Potator::CommandQueueComponent::Enqueue(std::shared_ptr<ICommand> command)
{
	_queue.push(command);
}

std::optional<std::shared_ptr<ICommand>> Potator::CommandQueueComponent::Dequeue()
{
	if (_queue.empty())
	{
		return std::optional<std::shared_ptr<ICommand>>();
	}

	std::shared_ptr<ICommand> next = _queue.front();
	_queue.pop();
	return next;
}
	
