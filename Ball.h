#pragma once
#include <id.h>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Color.hpp>
#include <math_functions.h>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <box2d.h>
#include <SFML/System/Vector2.hpp>
#include "BodyModel.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Time.hpp>

class Ball : public sf::Drawable
{
private:
	b2BodyId  m_BodyId;
	b2WorldId m_WorldId;

	float m_MaxSpeed;
	float m_Damage;	

	sf::CircleShape m_Sprite;

	bool m_Traveling = false;

	sf::VertexArray m_TrailVertexArray;
	int m_CurrentPoint = 0;

	float m_DelayBeforeTrailBeginsInSeconds = 0.1f;
	sf::Time m_TimeSinceLanch;

	int m_LaunchCount = 0;

	sf::Vector2f m_LastTrailPosition;
	float m_DistanceBetweenTrails = 50;

	void createBody(float radius, b2Vec2 startingPosition);
	void createSprite(float radius, sf::Color color);
	void spawnTrail();

public:
	Ball(float damage, float maxSpeed, float radius, sf::Color color, b2WorldId worldId);

	void launch(b2Vec2 startingPos, b2Vec2 normalizedDirection, float impulse);
	
	int getLaunchCount()
	{
		return m_LaunchCount;
	}

	void resetLaunchCount()
	{
		m_LaunchCount = 0;
	}

	void clearTrail()
	{
		m_TrailVertexArray.clear();
	}

	void setPosition(sf::Vector2f newPos)
	{
		m_Sprite.setPosition(newPos);
	}

	b2BodyId getBodyId() const
	{
		return m_BodyId;
	}

	sf::CircleShape& getSprite()
	{
		return m_Sprite;
	}

	bool applyDamage(BodyModel* target, float approachSpeed);

	void setWorldId(b2WorldId worldId);
	void update();
	void reset()
	{
		if (b2Body_IsValid(m_BodyId))
		{
			b2DestroyBody(m_BodyId);

			m_Traveling = false;
		}
	}
	
	void sleep()
	{
		b2Body_SetAwake(m_BodyId, false);
	}

	void awake()
	{
		b2Body_SetAwake(m_BodyId, true);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

