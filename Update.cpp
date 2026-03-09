#include "GameEngine.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>

void GameEngine::update(float delta)
{
	m_PhysicsEngine.update(delta);

	sf::Vector2f mousePosition = m_Window.mapPixelToCoords(sf::Mouse::getPosition());
	m_SlingShot.update(mousePosition);

	m_Ball.update();
}