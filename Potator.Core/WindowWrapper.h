#pragma once
#include <SFML/Window.hpp>

namespace Potator
{
	struct WindowWrapper
	{
		// bulshit wrapper needed for boost di
		sf::Window Window;
	};
}