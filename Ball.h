#pragma once
#include <id.h>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Color.hpp>
#include <math_functions.h>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class Ball : public sf::Drawable
{
private:
	b2BodyId  m_BodyId;
	b2WorldId m_WorldId;

	float m_MaxSpeed;
	float m_Impulse;
	float m_Damage;

	sf::CircleShape m_Sprite;

	void createBody(float radius, b2Vec2 startingPosition);
	void createSprite(float radius, sf::Color color);

public:
	Ball(float damage, float maxSpeed, float radius, b2Vec2 startingPosition, sf::Color color);

	void draw(sf::RenderTarget& target);
};

