#pragma once
#include <sfml/Window/Window.hpp>
#include "IInputHandler.h"
#include <boost/signals2.hpp>

namespace Potator
{
	class WindowHandler
	{
	public:
		WindowHandler(sf::Window* window);
		void Handle();
		void RegisterInputHandler(std::shared_ptr<IInputHandler> handler);
		boost::signals2::signal<void(unsigned int, unsigned int)> WindowResized;
	private:
		std::vector<std::shared_ptr<IInputHandler>> _inputHandlers;
		sf::Window* _window;
	};
}