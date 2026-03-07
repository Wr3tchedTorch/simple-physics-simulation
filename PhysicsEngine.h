#pragma once
#include <id.h>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <math_functions.h>
#include <SFML/System/Angle.hpp>
#include <types.h>

class PhysicsEngine
{
private:	
	const float GRAVITY	  = 9.8f;
	const float TIME_STEP = 1.0f / 60.0f;
	const int   SUB_STEP_COUNT = 4;

	b2WorldId m_WorldId;

	std::vector<b2BodyId> m_BodyIds;

	void cleanList();

public:
	PhysicsEngine();

	void spawnBodyAtLocation(sf::Vector2f location, sf::Vector2f size, sf::Angle rotation, sf::Color fillColor);
	void destroyBodyAtLocation(sf::Vector2f location);
	void destroyBody(b2BodyId body);

	void spawnBodyAtLocation(b2Vec2 location, b2Vec2 size, b2Rot rotation, sf::Color fillColor, b2BodyType type = b2_dynamicBody);
	void destroyBodyAtLocation(b2Vec2 location);

	void update(float delta);

	b2WorldId getWorld();
	std::vector<b2BodyId> getBodies();
};

