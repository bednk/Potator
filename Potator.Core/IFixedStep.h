#pragma once

namespace Potator
{
	class IFixedStep
	{
	public:
		virtual void SetTickRate(int tickRate) = 0;
		virtual void Update() = 0;
	};
}