#include "GameEngine.h"
#include <SFML/System/Clock.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Angle.hpp>
#include <math_functions.h>
#include <SFML/System/Vector2.hpp>
#include <types.h>
#include "Converter.h"
#include "BodyModel.h"
#include <memory>

GameEngine::GameEngine()
{
	sf::VideoMode vm = sf::VideoMode::getDesktopMode();
	m_Resolution = sf::Vector2f(vm.size);
	
	m_Window.create(vm, "Physics Simulation by Eric");
	m_Window.setFramerateLimit(60);

	sf::Angle angle = sf::degrees(45);
	b2Rot rot = b2MakeRot(angle.asRadians());

	BodyModel model;
	model.m_Color = sf::Color::White;

	m_PhysicsEngine.spawnBodyAtLocation(
		{
		converter::pixelsToMeters(m_Resolution.x) / 2.0f,
		converter::pixelsToMeters(m_Resolution.y) / 1.25f + 2.5f
		},
		{
			converter::pixelsToMeters(m_Resolution.x),
			5
		},
		b2Rot_identity, model, b2_staticBody);
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
