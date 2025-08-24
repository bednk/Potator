#pragma once

namespace Potator
{
	class IInputHandler
	{
	public:
		virtual ~IInputHandler() = default;
		virtual void Handle() = 0;
		virtual void SetEntity(Entity entity) = 0;
	};
}
