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
#include "Ball.h"
#include <memory>
#include "PhysicsEngine.h"

bool GameEngine::IsEditMode = false;
sf::Vector2f GameEngine::Resolution = {0, 0};

GameEngine::GameEngine() :
	m_SlingShot(4, 30, {400, 1080.0f/2.0f}),
	m_Ball(180.0f, 2, 1, sf::Color::Red, m_PhysicsEngine->getWorld()),
	m_PhysicsEngine(std::make_shared<PhysicsEngine>()),
	m_BoxFactory(m_PhysicsEngine)
{
	sf::VideoMode vm = sf::VideoMode::getDesktopMode();
	Resolution = sf::Vector2f(vm.size);
	
	m_Window.create(vm, "Physics Simulation by Eric");
	m_Window.setFramerateLimit(60);

	sf::Angle angle = sf::degrees(45);
	b2Rot rot = b2MakeRot(angle.asRadians());

	BodyModel model;
	model.m_Color = sf::Color::White;

	m_PhysicsEngine->spawnBodyAtLocation(
		{
		converter::pixelsToMeters(Resolution.x) / 2.0f,
		converter::pixelsToMeters(Resolution.y) / 1.25f + 2.5f
		},
		{
			converter::pixelsToMeters(Resolution.x),
			5
		},
		b2Rot_identity, model, b2_staticBody);	

	m_Ball.setWorldId(m_PhysicsEngine->getWorld());
	m_SlingShot.setAmmo(m_Ball);
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
