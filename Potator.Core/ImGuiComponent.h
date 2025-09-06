#pragma once
#include <functional>

namespace Potator
{
	struct ImGuiComponent
	{
		std::function<void()> Draw;
	};
}