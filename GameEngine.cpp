#include "GameEngine.h"
#include <SFML/System/Clock.hpp>
#include <SFML/Window/VideoMode.hpp>

GameEngine::GameEngine()
{
	sf::VideoMode vm = sf::VideoMode::getDesktopMode();
	m_Resolution = vm.size;
	
	m_Window.create(vm, "Physics Simulation by Eric");
	m_Window.setFramerateLimit(60);
}

void GameEngine::run()
{
	sf::Clock clock;
	while (m_Window.isOpen())
	{
		float delta = clock.restart().asSeconds();

		input();
		update(delta);
		draw();
	}
}
