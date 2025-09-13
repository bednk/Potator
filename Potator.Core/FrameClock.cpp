#include "FrameClock.h"

void Potator::FrameClock::Update()
{
	auto now = Clock::now();
	if (!_lastTime)
	{
		_lastTime = now;
		return;
	}

	_frameTime = now - _lastTime.value();
	_lastTime = now;
}

Duration Potator::FrameClock::GetFrameTime()
{
	return _frameTime;
}
