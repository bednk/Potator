#include "CommandQueueComponent.h"

Potator::CommandQueueComponent::CommandQueueComponent(Entity owner) :
	_owner { owner }
{
}

void Potator::CommandQueueComponent::Enqueue(ICommand* command)
{
	_queue.push(command);
}

std::optional<Potator::ICommand*> Potator::CommandQueueComponent::Dequeue()
{
	if (_queue.empty())
	{
		return std::optional<ICommand*>();
	}

	ICommand* next = _queue.front();
	_queue.pop();
	return next;
}
	
