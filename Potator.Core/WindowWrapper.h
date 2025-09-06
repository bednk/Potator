#pragma once
#include <GLFW/glfw3.h>

namespace Potator
{
	struct WindowWrapper
	{
		// bulshit wrapper needed for boost di
		GLFWwindow* Window;
	};
}