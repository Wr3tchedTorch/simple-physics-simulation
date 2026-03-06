#include "GameEngine.h"
#include <SFML/Window/Event.hpp>

void GameEngine::input()
{
	while (auto event = m_Window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			m_Window.close();
		}
	}
}