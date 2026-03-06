#pragma once
#include <id.h>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <math_functions.h>

struct PointOverlapResultQueryContext
{
	b2Vec2 m_Point;
	b2Vec2 m_Point;
};

class PhysicsEngine
{
private:	
	const float GRAVITY	  = 9.8f;
	const float TIME_STEP = 1.0f / 60.0f;
	const int   SUB_STEP_COUNT = 4;

	b2WorldId m_WorldId;

	std::vector<b2BodyId> m_BodyIds;

	void removeDestroyedBodyFromList(b2BodyId body);
	void cleanList();

	bool b2OverlapResultFcn(b2ShapeId shapeId, void* context)
	{

	}
public:
	PhysicsEngine();

	void spawnBodyAtLocation(sf::Vector2f location, sf::Vector2f size, sf::Color fillColor);
	void destroyBodyAtLocation(sf::Vector2f location);

	void spawnBodyAtLocation(b2Vec2 location, b2Vec2 size, sf::Color fillColor);
	void destroyBodyAtLocation(b2Vec2 location);

	void update(float delta);

	b2WorldId getWorld();
	std::vector<b2BodyId> getBodies();
};

