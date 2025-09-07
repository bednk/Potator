#pragma once
#include "IHwndProvider.h"
#include "IGlfwWindowProvider.h"
#include "LaunchingParams.h"


namespace Potator
{
	class Window : public IHwndProvider, public IGlfwWindowProvider
	{
	public:
		Window(const LaunchingParams& params);
		HWND GetHWND() const override;
		GLFWwindow* GetGlfwWindow() const override;
		bool IsRunning();
		~Window();
	private:
		GLFWwindow* _window;
	};
}