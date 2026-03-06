#include "GameEngine.h"
#include <SFML/System/Clock.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Angle.hpp>
#include <math_functions.h>
#include <numbers>

GameEngine::GameEngine()
{
	sf::VideoMode vm = sf::VideoMode::getDesktopMode();
	m_Resolution = vm.size;
	
	m_Window.create(vm, "Physics Simulation by Eric");
	m_Window.setFramerateLimit(60);

	sf::Angle angle = sf::degrees(135);
	b2Rot rot = b2MakeRot(angle.asRadians());

	m_PhysicsEngine.spawnBodyAtLocation({ 2, 2 }, {40, 40}, rot, sf::Color::Red);
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
