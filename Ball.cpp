#include "Ball.h"
#include <SFML/Graphics/RenderStates.hpp>
#include <types.h>
#include <box2d.h>
#include <collision.h>
#include <math_functions.h>
#include <SFML/Graphics/Color.hpp>
#include "Converter.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

void Ball::createBody(float radius, b2Vec2 startingPosition)
{
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = startingPosition;
	bodyDef.isBullet = true;
	bodyDef.userData = this;
	bodyDef.fixedRotation = true;

	m_BodyId = b2CreateBody(m_WorldId, &bodyDef);

	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 1.0f;
	shapeDef.material.restitution = 0.2f;
	shapeDef.material.friction = 0.3f;

	b2Circle circle{};
	circle.radius = radius;

	b2CreateCircleShape(m_BodyId, &shapeDef, &circle);
}

void Ball::createSprite(float radius, sf::Color color)
{
	m_Sprite.setRadius(radius);
	m_Sprite.setFillColor(color);
}

Ball::Ball(float damage, float maxSpeed, float radius, b2Vec2 startingPosition, sf::Color color)
{
	m_Impulse  = 0;
	m_Damage   = damage;
	m_MaxSpeed = maxSpeed;	

	createBody(radius, startingPosition);
	createSprite(radius, color);

}

void Ball::draw(sf::RenderTarget& target)
{
	b2Vec2 position = b2Body_GetPosition(m_BodyId);

	sf::Vector2f positionInPixels(
		{
			converter::metersToPixels(position.x),
			converter::metersToPixels(position.y)
		}
	);

	m_Sprite.setPosition(positionInPixels);

	target.draw(m_Sprite);
}
