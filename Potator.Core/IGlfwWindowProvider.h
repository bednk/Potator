#pragma once
#include <GLFW/glfw3.h>

namespace Potator
{
	class IGlfwWindowProvider
	{
	public:
		~IGlfwWindowProvider() = default;
		virtual GLFWwindow* GetGlfwWindow() const = 0;
	};
}