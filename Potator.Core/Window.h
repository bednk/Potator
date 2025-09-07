#pragma once
#include "IHwndProvider.h"
#include <GLFW/glfw3.h>
#include "LaunchingParams.h"


namespace Potator
{
	class Window : public IHwndProvider
	{
	public:
		Window(const LaunchingParams& params);
		HWND GetHWND() const override;
		GLFWwindow* GetGlfwWindow() const;
		bool IsRunning();
		~Window();
	private:
		GLFWwindow* _window;
	};
}