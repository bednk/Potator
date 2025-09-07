#pragma once
#include "leanWin.h"

namespace Potator
{
	class IHwndProvider
	{
	public:
		virtual HWND GetHWND() const = 0;
	};
}