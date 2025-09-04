#include "WindowHandler.h"

Potator::WindowHandler::WindowHandler(WindowWrapper& window) : _window { window.Window }
{
}

void Potator::WindowHandler::Handle()
{
	while (const std::optional event = _window.pollEvent())
	{
		if (!event)
		{
			continue;
		}

		if (event->is<sf::Event::Closed>())
		{
			_window.close();
		}

		else if (event->is<sf::Event::Resized>())
		{
			const auto* resizeEvent = event->getIf<sf::Event::Resized>();
			WindowResized(resizeEvent->size.x, resizeEvent->size.y);
		}

		for (size_t i = 0; i < _inputHandlers.size(); i++)
		{
			_inputHandlers[i]->Handle();
		}
	}
}

void Potator::WindowHandler::RegisterInputHandler(std::shared_ptr<IInputHandler> handler)
{
	_inputHandlers.push_back(handler);
}
