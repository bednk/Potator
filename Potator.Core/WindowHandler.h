#pragma once
#include "IInputHandler.h"
#include "Window.h"
#include <boost/signals2.hpp>

namespace Potator
{
	class WindowHandler
	{
	public:
		WindowHandler(IGlfwWindowProvider& window);
		bool Handle();
		void RegisterInputHandler(std::shared_ptr<IInputHandler> handler);
		boost::signals2::signal<void(unsigned int, unsigned int)> WindowResized;
	private:
		std::vector<std::shared_ptr<IInputHandler>> _inputHandlers;
		GLFWwindow* _window;
		int _width = 0;
		int _height = 0;
	};
}