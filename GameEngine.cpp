#include "GameEngine.h"
#include <SFML/System/Clock.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <math_functions.h>
#include <SFML/System/Vector2.hpp>
#include <types.h>
#include "Converter.h"
#include "BodyModel.h"
#include "Ball.h"
#include <memory>
#include "PhysicsEngine.h"
#include "BoxFactory.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Font.hpp>
#include <random>
#include <cstdint>

bool GameEngine::IsEditMode = false;
sf::Vector2f GameEngine::Resolution = {0, 0};
sf::Time GameEngine::GameTimeTotal;
int GameEngine::Score = 0;

GameEngine::GameEngine() :
	m_SlingShot(4, 40, {400, 1080.0f/2.0f + 130}),
	m_PhysicsEngine(std::make_shared<PhysicsEngine>()),
	m_BoxFactory(m_PhysicsEngine),
	m_LevelManager(m_PhysicsEngine),
	m_HUD(sf::Font("fonts/Astroz Trial.ttf"), sf::Font("fonts/Vipnagorgialla Rg.otf"))
{
	sf::VideoMode vm = sf::VideoMode::getDesktopMode();
	Resolution = sf::Vector2f(vm.size);
	
	m_Window.create(vm, "Physics Simulation by Eric");
	m_Window.setFramerateLimit(60);

	m_HUDView.setSize(Resolution);
	m_HUDView.setViewport(
		{
			{.025f, .025f},
			{.95f, .95f}
		});
	m_HUDView.setCenter(
		{
			Resolution.x / 2.0f,
			Resolution.y / 2.0f
		});

	m_GameView.setSize(Resolution);
	m_GameView.setCenter(
		{
			Resolution.x / 2.0f,
			Resolution.y / 2.0f
		});
	m_GameView.zoom(1.5);

	sf::FloatRect globalBounds({
		m_GameView.getCenter() - m_GameView.getSize() / 2.0f,
		m_GameView.getSize()
	});

	m_BoxFactory.setGlobalBounds(globalBounds);

	m_SlingShot.setGlobalBounds(globalBounds);

	m_SlingShot.setStartingBallPosition({
		-converter::metersToPixels(5),
		m_GameView.getCenter().y + m_GameView.getSize().y / 2.0f - converter::metersToPixels(5) - 250
	});

	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<uint32_t> dist(0, 255);

	for (int i = 0; i < 3; i++)
	{
		auto& ball = m_Balls.emplace_back(std::make_unique<Ball>());
		ball->init(180.0f, 2, 1, sf::Color(dist(gen), dist(gen), dist(gen)), m_PhysicsEngine->getWorld());
		ball->setPosition({
			-converter::metersToPixels(7.0f + i * 2),
			m_GameView.getCenter().y + m_GameView.getSize().y / 2.0f - converter::metersToPixels(5) - ball->getSprite().getRadius()/2.0f
		});
		ball->setWorldBounds(globalBounds);
	}

	m_SlingShot.setBalls(m_Balls);

	m_LevelManager.loadLevel(1);
	spawnGround();
}

void GameEngine::spawnGround()
{
	BodyModel model;
	model.m_Color = sf::Color::White;

	m_PhysicsEngine->spawnBodyAtLocation(
		{
			converter::pixelsToMeters(m_GameView.getCenter().x),
			converter::pixelsToMeters(m_GameView.getCenter().y + m_GameView.getSize().y / 2.0f - converter::metersToPixels(2.5f))
		},
		{
			converter::pixelsToMeters(m_GameView.getSize().x),
			5.0f
		},
		b2Rot_identity, model, b2_staticBody);
}

void GameEngine::run()
{
	sf::Clock clock;
	while (m_Window.isOpen())
	{
		sf::Time deltaTime = clock.restart();
		float delta = deltaTime.asSeconds();
		GameTimeTotal += deltaTime;

		input();
		update(delta);
		draw();
	}
}
