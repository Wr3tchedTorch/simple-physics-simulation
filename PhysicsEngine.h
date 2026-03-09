#pragma once
#include <id.h>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include <math_functions.h>
#include <SFML/System/Angle.hpp>
#include <types.h>
#include "BodyModel.h"
#include <memory>
#include <unordered_map>
#include <functional>

class PhysicsEngine
{
private:	
	const float GRAVITY	  = 9.8f;
	const float TIME_STEP = 1.0f / 60.0f;
	const int   SUB_STEP_COUNT = 4;

	b2WorldId m_WorldId;
	
	std::unordered_map<b2BodyId, std::unique_ptr<BodyModel>, b2BodyIdHash, b2BodyIdEqual> m_Bodies;

	void cleanList();

public:
	PhysicsEngine();

	void spawnBodyAtLocation(sf::Vector2f location, sf::Vector2f size, sf::Angle rotation, BodyModel model, b2BodyType type = b2_dynamicBody);
	void destroyBodyAtLocation(sf::Vector2f location);
	void destroyBody(b2BodyId body);

	void spawnBodyAtLocation(b2Vec2 location, b2Vec2 size, b2Rot rotation, BodyModel model, b2BodyType type = b2_dynamicBody);
	void destroyBodyAtLocation(b2Vec2 location);

	void update(float delta);

	b2WorldId getWorld();

	void forEachBody(std::function<void(b2BodyId, const BodyModel&)> fn) const 
	{
		for (const auto& [id, body] : m_Bodies)
		{
			fn(id, *body);
		}
	}
};

