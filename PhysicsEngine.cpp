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

PhysicsEngine::PhysicsEngine()
{
	b2WorldDef worldDef = b2DefaultWorldDef();
	worldDef.gravity = { 0, GRAVITY };

	m_WorldId = b2CreateWorld(&worldDef);
}

void PhysicsEngine::spawnBodyAtLocation(sf::Vector2f location, sf::Vector2f size, sf::Color fillColor)
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

	spawnBodyAtLocation(locationInMeters, sizeInMeters, fillColor);
}

void PhysicsEngine::destroyBodyAtLocation(sf::Vector2f location)
{
	b2Vec2 locationInMeters =
	{
		converter::pixelsToMeters(location.x),
		converter::pixelsToMeters(location.y)
	};

	destroyBodyAtLocation(locationInMeters);
}

void PhysicsEngine::spawnBodyAtLocation(b2Vec2 location, b2Vec2 size, sf::Color fillColor)
{
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.position  = location;
	bodyDef.type = b2_dynamicBody;

	b2BodyId bodyId = b2CreateBody(m_WorldId, &bodyDef);

	b2Polygon  polygon  = b2MakeBox(size.x / 2.0f, size.y / 2.0f);
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	b2CreatePolygonShape(bodyId, &shapeDef, &polygon);

	m_BodyIds.push_back(bodyId);
}

void PhysicsEngine::destroyBodyAtLocation(b2Vec2 location)
{
	b2ShapeProxy proxy = b2MakeProxy(new b2Vec2(location), 1, 0.001f);

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
		#ifdef _DEBUG
		std::cout << std::format("\nFound shape at position: (x: {}, y: {})", location.x, location.y);
		#endif

		b2BodyId bodyId = b2Shape_GetBody(foundShapeId);
		b2DestroyBody(bodyId);

		removeDestroyedBodyFromList(body);
	}
}

void PhysicsEngine::update(float delta)
{
	b2World_Step(m_WorldId, TIME_STEP, SUB_STEP_COUNT);
}
