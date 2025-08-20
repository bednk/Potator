#pragma once

namespace Potator
{
	class IInputHandler
	{
	public:
		virtual ~IInputHandler() = default;
		virtual void Handle() = 0;
	};
}
