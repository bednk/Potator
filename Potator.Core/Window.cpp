#include "Window.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h> 

Potator::Window::Window(const LaunchingParams& params)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	_window = glfwCreateWindow(params.Width, params.Height, params.Title.c_str(), nullptr, nullptr);
}

HWND Potator::Window::GetHWND() const
{
	return glfwGetWin32Window(_window);
}

GLFWwindow* Potator::Window::GetGlfwWindow() const
{
	return _window;
}

bool Potator::Window::IsRunning()
{
	return !glfwWindowShouldClose(_window);
}

Potator::Window::~Window()
{
	glfwDestroyWindow(_window);
	glfwTerminate();
}
