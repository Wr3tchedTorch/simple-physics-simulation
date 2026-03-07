#include "PhysicsEngine.h"
#include <types.h>
#include <box2d.h>
#include <SFML/System/Vector2.hpp>
#include "Converter.h"
#include <math_functions.h>
#include <SFML/Graphics/Color.hpp>
#include <collision.h>
#include <id.h>
#include <iostream>
#include <format>
#include <vector>
#include <SFML/System/Angle.hpp>

void PhysicsEngine::destroyBody(b2BodyId bodyId)
{
	for (int i = 0; i < m_BodyIds.size(); ++i)
	{
		auto id = m_BodyIds.at(i);
		if (B2_ID_EQUALS(id, bodyId))
		{
			b2DestroyBody(id); 
			m_BodyIds.erase(m_BodyIds.begin() + i);
			return;
		}
	}
}

void PhysicsEngine::cleanList()
{
	for (auto id : m_BodyIds)
	{
		b2DestroyBody(id);
	}
	m_BodyIds.clear();
}

PhysicsEngine::PhysicsEngine()
{
	b2WorldDef worldDef = b2DefaultWorldDef();
	worldDef.gravity = { 0, GRAVITY };

	m_WorldId = b2CreateWorld(&worldDef);
}

void PhysicsEngine::spawnBodyAtLocation(sf::Vector2f location, sf::Vector2f size, sf::Angle rotation, sf::Color fillColor)
{
	b2Vec2 locationInMeters =
	{
		converter::pixelsToMeters(location.x),
		converter::pixelsToMeters(location.y)
	};
	b2Vec2 sizeInMeters =
	{
		converter::pixelsToMeters(size.x),
		converter::pixelsToMeters(size.y)
	};
	b2Rot rotationB2;	
	rotationB2 = b2MakeRot(rotation.asRadians());

	spawnBodyAtLocation(locationInMeters, sizeInMeters, rotationB2, fillColor);
}

void PhysicsEngine::destroyBodyAtLocation(sf::Vector2f location)
{
	b2Vec2 locationInMeters =
	{
		converter::pixelsToMeters(location.x),
		converter::pixelsToMeters(location.y - 20)
	};

	destroyBodyAtLocation(locationInMeters);
}

void PhysicsEngine::spawnBodyAtLocation(b2Vec2 location, b2Vec2 size, b2Rot rotation, sf::Color fillColor, b2BodyType type)
{
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.position  = location;
	bodyDef.rotation  = rotation;
	bodyDef.type	  = type;
	if (type == b2_dynamicBody)
	{		
		bodyDef.angularVelocity = converter::degToRad(60);	
	}

	b2BodyId bodyId = b2CreateBody(m_WorldId, &bodyDef);	

	b2Polygon  polygon  = b2MakeBox(size.x / 2.0f, size.y / 2.0f);
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	if (type == b2_dynamicBody)
	{
		shapeDef.density = 1.0f;
		shapeDef.material.friction = .3f;
	}
	b2CreatePolygonShape(bodyId, &shapeDef, &polygon);

	m_BodyIds.push_back(bodyId);

	#ifdef _DEBUG
	std::cout << std::format("\nPlaced shape at position: (x: {}, y: {}); size: (x: {}, y: {})", location.x, location.y, size.x, size.y);
	#endif
}

void PhysicsEngine::destroyBodyAtLocation(b2Vec2 location)
{	
	b2ShapeProxy proxy = b2MakeProxy(new b2Vec2(location), 1, .2f);

	b2ShapeId foundShapeId = b2_nullShapeId;

	b2World_OverlapShape(
		m_WorldId, 
		&proxy, 
		b2DefaultQueryFilter(), 
		[](b2ShapeId shapeId, void* context) 
		{
			*static_cast<b2ShapeId*>(context) = shapeId;

			return false;
		}, 
		&foundShapeId);

	if (b2Shape_IsValid(foundShapeId))
	{
		b2BodyId bodyId = b2Shape_GetBody(foundShapeId);

		if (b2Body_GetType(bodyId) == b2_dynamicBody)
		{
			#ifdef _DEBUG
			std::cout << std::format("\nDestroyed shape at position: (x: {}, y: {})", location.x, location.y);
			#endif

			destroyBody(bodyId);
		}
	}
}

void PhysicsEngine::update(float delta)
{
	b2World_Step(m_WorldId, TIME_STEP, SUB_STEP_COUNT);
}

b2WorldId PhysicsEngine::getWorld()
{
	return m_WorldId;
}

std::vector<b2BodyId> PhysicsEngine::getBodies()
{
	return m_BodyIds;
}
