#include "Ball.h"
#include <types.h>
#include <box2d.h>
#include <collision.h>
#include <math_functions.h>
#include <SFML/Graphics/Color.hpp>
#include "Converter.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <id.h>
#include <iostream>
#include <format>
#include "BodyModel.h"

void Ball::createBody(float radius, b2Vec2 startingPosition)
{
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = startingPosition;
	bodyDef.isBullet = true;
	bodyDef.userData = this;
	bodyDef.fixedRotation = true;
	bodyDef.isAwake = false;

	m_BodyId = b2CreateBody(m_WorldId, &bodyDef);

	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 1.0f;
	shapeDef.material.restitution = 0.2f;
	shapeDef.material.friction    = 0.3f;
	shapeDef.enableHitEvents  = true;

	b2Circle circle{};
	circle.radius = radius;

	b2CreateCircleShape(m_BodyId, &shapeDef, &circle);	
}

void Ball::createSprite(float radius, sf::Color color)
{
	m_Sprite.setRadius(converter::metersToPixels(radius));
	m_Sprite.setFillColor(color);
	m_Sprite.setOrigin({ converter::metersToPixels(radius), converter::metersToPixels(radius) });
	m_Sprite.setPosition({-200, -200});
}

Ball::Ball(float damage, float maxSpeed, float radius, sf::Color color, b2WorldId worldId)
{
	m_WorldId  = worldId;
	m_Damage   = damage;
	m_MaxSpeed = maxSpeed;

	createSprite(radius, color);
}

void Ball::launch(b2Vec2 startingPos, b2Vec2 normalizedDirection, float impulse)
{
	if (!b2Body_IsValid(m_BodyId))
	{
		createBody(converter::pixelsToMeters(m_Sprite.getRadius()), startingPos);
	}

	b2Vec2 velocity = normalizedDirection * impulse * m_MaxSpeed;

	b2Body_SetAwake(m_BodyId, true);
	b2Body_SetLinearVelocity(m_BodyId, velocity);
}

void Ball::applyDamage(BodyModel* target, float approachSpeed)
{

	const float MIN_SPEED = 4.0f;
	const float MAX_SPEED = 60.0f;
	float impactPower = (approachSpeed - MIN_SPEED) / (MAX_SPEED - MIN_SPEED);
	impactPower = b2ClampFloat(impactPower, 0.0f, 1.0f);

	float curvedPower = impactPower * impactPower;

	float totalDamage = curvedPower * m_Damage * target->m_MaterialDamageMultiplier;

	#ifdef _DEBUG
	std::cout << std::format("\napplying damage to a box, approach speed: {}, damage: {}, total damage: {}", approachSpeed, m_Damage, totalDamage);
	#endif

	target->m_Health -= totalDamage;
	if (target->m_Health <= 0)
	{
		b2DestroyBody(target->m_Id);
	}
}

void Ball::setWorldId(b2WorldId worldId)
{
	m_WorldId = worldId;
}

void Ball::update()
{
	if (!b2Body_IsValid(m_BodyId))
	{
		return;
	}

	b2Vec2 position = b2Body_GetPosition(m_BodyId);

	sf::Vector2f positionInPixels(
		{
			converter::metersToPixels(position.x),
			converter::metersToPixels(position.y)
		}
	);

	m_Sprite.setPosition(positionInPixels);
}

void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Sprite, states);
}
