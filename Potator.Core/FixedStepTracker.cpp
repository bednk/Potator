#include "FixedStepTracker.h"

Potator::FixedStepTracker::FixedStepTracker() : FixedStepTracker(100)
{
}

Potator::FixedStepTracker::FixedStepTracker(int tickRate) :
	_tickRate { tickRate },
	_fixedStep{ std::chrono::duration_cast<Duration>(std::chrono::duration<double>(1.0 / tickRate)) },
	_accumulator { Duration::zero() }
{

}

void Potator::FixedStepTracker::OnFrameStart()
{
	auto now = Clock::now();
	if (!_lastTime)
	{
		_lastTime = now;
		return;
	}

	_accumulator += now - _lastTime.value();
	_lastTime = now;
}

void Potator::FixedStepTracker::Subscribe(IFixedStep* subscriber)
{
	subscriber->SetTickRate(_tickRate);
	_subscribers.push_back(subscriber);
}

void Potator::FixedStepTracker::Update()
{
	while (_accumulator >= _fixedStep)
	{
		for (size_t i = 0; i < _subscribers.size(); i++)
		{
			_subscribers[i]->Update();
		}
		_accumulator -= _fixedStep;
	}
}
