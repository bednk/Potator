#include "FixedStepTracker.h"

Potator::FixedStepTracker::FixedStepTracker(LaunchingParams params, FrameClock& clock) :
	_tickRate{ params.FixedStepRate },
	_fixedStep{ std::chrono::duration_cast<Duration>(std::chrono::duration<double>(1.0 / _tickRate)) },
	_clock { clock }
{
}

void Potator::FixedStepTracker::Subscribe(IFixedStep* subscriber)
{
	subscriber->SetTickRate(_tickRate);
	_subscribers.push_back(subscriber);
}

void Potator::FixedStepTracker::Update()
{
	_accumulator += _clock.GetFrameTime();

	while (_accumulator >= _fixedStep)
	{
		for (size_t i = 0; i < _subscribers.size(); i++)
		{
			_subscribers[i]->Update();
		}
		_accumulator -= _fixedStep;
	}
}
