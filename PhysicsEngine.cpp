#include "PhysicsEngine.h"
#include <types.h>
#include <box2d.h>
#include <SFML/System/Vector2.hpp>
#include "Converter.h"
#include <math_functions.h>
#include <collision.h>
#include <id.h>
#include <iostream>
#include <format>
#include <vector>
#include <SFML/System/Angle.hpp>
#include "BodyModel.h"
#include <utility>
#include <memory>
#include <unordered_map>

void PhysicsEngine::cleanList()
{
	for (auto& [id, body] : m_Bodies)
	{
		b2DestroyBody(id);
	}
	m_Bodies.clear();
}

PhysicsEngine::PhysicsEngine()
{
	b2WorldDef worldDef = b2DefaultWorldDef();
	worldDef.gravity = { 0, GRAVITY };

	m_WorldId = b2CreateWorld(&worldDef);
}

void PhysicsEngine::spawnBodyAtLocation(sf::Vector2f location, sf::Vector2f size, sf::Angle rotation, BodyModel model, b2BodyType type)
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

	spawnBodyAtLocation(locationInMeters, sizeInMeters, rotationB2, model, type);
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

void PhysicsEngine::destroyBody(b2BodyId body)
{ 
	b2DestroyBody(body);

	m_Bodies.erase(body);
}

void PhysicsEngine::spawnBodyAtLocation(b2Vec2 location, b2Vec2 size, b2Rot rotation, BodyModel model, b2BodyType type)
{

	std::unique_ptr<BodyModel> modelPtr = std::make_unique<BodyModel>(model);

	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.position  = location;
	bodyDef.rotation  = rotation;
	bodyDef.type	  = type;
	bodyDef.userData  = modelPtr.get();	

	b2BodyId bodyId = b2CreateBody(m_WorldId, &bodyDef);	
	m_Bodies[bodyId] = std::move(modelPtr);
	m_Bodies[bodyId]->m_Id = bodyId;

	b2Polygon  polygon  = b2MakeBox(size.x / 2.0f, size.y / 2.0f);
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	
	if (type == b2_dynamicBody)
	{
		shapeDef.density = 1.0f;
		shapeDef.material.friction   = .3f;		
	}
	b2CreatePolygonShape(bodyId, &shapeDef, &polygon);	

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
