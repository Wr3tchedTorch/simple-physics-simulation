#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include "PhysicsEngine.h"
#include "Ball.h"
#include "SlingShot.h"

class GameEngine
{
private:
	sf::RenderWindow m_Window;
	sf::Vector2f	 m_Resolution;	

	PhysicsEngine m_PhysicsEngine;

	Ball m_Ball;
	SlingShot m_SlingShot;

	void update(float delta);
	void input();
	void draw();

public:
	GameEngine();

	void run();
};

