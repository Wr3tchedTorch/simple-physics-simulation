#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include "PhysicsEngine.h"

class GameEngine
{
private:
	sf::RenderWindow m_Window;
	sf::Vector2u	 m_Resolution;	

	PhysicsEngine m_PhysicsEngine;

	void update(float delta);
	void input();
	void draw();

public:
	GameEngine();

	void run();
};

