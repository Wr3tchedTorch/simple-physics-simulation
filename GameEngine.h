#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include "PhysicsEngine.h"
#include "Ball.h"
#include "SlingShot.h"
#include <memory>
#include "BoxFactory.h"
#include "LevelManager.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Time.hpp>
#include "HUD.h"

class GameEngine
{
private:
	sf::RenderWindow m_Window;

	std::shared_ptr<PhysicsEngine> m_PhysicsEngine;

	Ball m_Ball;
	SlingShot m_SlingShot;
	BoxFactory m_BoxFactory;
	LevelManager m_LevelManager;
	HUD m_HUD;

	sf::View m_GameView;
	sf::View m_HUDView;

	sf::Keyboard::Scancode m_LastPressedKey;

	void update(float delta);
	void input();
	void draw();

public:
	static sf::Time GameTimeTotal;
	static bool IsEditMode;
	static sf::Vector2f	Resolution;	
	static int Score;

	GameEngine();

	void spawnGround();

	void run();
};

