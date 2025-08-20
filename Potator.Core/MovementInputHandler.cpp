#include "MovementInputHandler.h"
#include <SFML/Window/Keyboard.hpp>

Potator::MovementInputHandler::MovementInputHandler(CommandDispatcher& commandDispatcher, MovementSystem& movementSystem) :
	_entity { NONE_ENTITY },
	_commandDispatcher { commandDispatcher },
	_command { movementSystem }
{
}

void Potator::MovementInputHandler::SetEntity(Entity entity)
{
	_entity = entity;
}

void Potator::MovementInputHandler::Handle()
{
	if (_entity == NONE_ENTITY)
	{
		return;
	}

	_command.LinearVelocity.x() =
		(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ? -_linerUnitsPerS : 0) +
		(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ? _linerUnitsPerS : 0);
	_command.LinearVelocity.y() =
		(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) ? -_linerUnitsPerS : 0) +
		(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) ? _linerUnitsPerS : 0);
	_command.LinearVelocity.z() =
		(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ? -_linerUnitsPerS : 0) +
		(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ? _linerUnitsPerS : 0);

	_command.AngularVelocity.x() =
		(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J) ? -_angilarRadiansPerS : 0) +
		(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::U) ? _angilarRadiansPerS : 0);
	_command.AngularVelocity.y() =
		(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y) ? -_angilarRadiansPerS : 0) +
		(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I) ? _angilarRadiansPerS : 0);
	_command.AngularVelocity.z() =
		(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H) ? -_angilarRadiansPerS : 0) +
		(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K) ? _angilarRadiansPerS : 0);

	_commandDispatcher.Enqueue(_entity, &_command);
}
