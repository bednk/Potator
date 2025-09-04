#pragma once
#include <windef.h>

namespace Potator
{
	struct WindowHandle
	{
		WindowHandle(HWND handle) : Handle{ handle } {};
		HWND Handle;
	};
}