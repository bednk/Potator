#pragma once
#include "leanWin.h"

namespace Potator
{
	class IHwndProvider
	{
	public:
		~IHwndProvider() = default;
		virtual HWND GetHWND() const = 0;
	};
}