#pragma once
#include <SFML/System/Vector2.hpp>
#include "Ball.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <memory>
#include <vector>
#include <id.h>
#include <box2d.h>
#include <math_functions.h>
#include <cstdlib>

class SlingShot : public sf::Drawable
{
private:
	sf::Vector2f m_DragMousePosition;
	sf::Vector2f m_CurrentMousePosition;
	float m_MaxDragDistance;
	float m_MaxImpulse;

	bool m_IsDragging = false;
	bool m_AnyBallsLeft = true;
	bool m_LaunchedCurrentBall = false;
	bool m_ReloadedCurrentBall = false;

	sf::Vector2f m_LaunchDirection;
	sf::Vector2f m_StartingBallPosition;
	sf::FloatRect m_GlobalBounds;

	sf::RectangleShape m_SlingshotRect;
	sf::RectangleShape m_SlingshotBaseRect;
	
	std::vector<std::shared_ptr<Ball>> m_Balls;
	int m_CurrentBallIndex = -1;

public:
	SlingShot(float maxDragDistance, float maxImpulse, sf::Vector2f startingBallPosition);

	void setBalls(std::vector<std::shared_ptr<Ball>> balls)
	{
		m_Balls = balls;

		loadNextBall();
	}

	bool isBall(b2BodyId id)
	{
		for (auto& ball : m_Balls)
		{
			if (B2_ID_EQUALS(id, ball->getBodyId()))
			{
				return true;
			}
		}
		return false;
	}

	void reload()
	{
		m_CurrentBallIndex = -1;
		m_AnyBallsLeft = true;

		loadNextBall();
	}

	std::vector<std::shared_ptr<Ball>> getActiveBalls()
	{
		
	}

	bool allBallsStopped()
	{		
		bool result = true;
		for (auto& ball : m_Balls)
		{
			if (!ball->isActive())
			{
				continue;
			}

			if (ball == nullptr || !b2Body_IsValid(ball->getBodyId()))
			{
				return false;
			}

			if (b2Body_IsAwake(ball->getBodyId()))
			{
				result = false;
			}
		}
		return result;
	}

	void setStartingBallPosition(sf::Vector2f position)
	{
		m_StartingBallPosition = position;
		m_SlingshotBaseRect.setPosition(position);
		m_SlingshotRect.setPosition(position);
	}

	void setGlobalBounds(sf::FloatRect globalBounds)
	{
		m_GlobalBounds = globalBounds;
	}

	void loadNextBall()
	{
		if (!m_AnyBallsLeft)
		{
			return;
		}

		m_CurrentBallIndex++;
		if (m_CurrentBallIndex == m_Balls.size())
		{
			m_AnyBallsLeft = false;
			return;
		}
		m_Balls.at(m_CurrentBallIndex)->setPosition(m_StartingBallPosition);
		m_LaunchedCurrentBall = false;
		m_ReloadedCurrentBall = false;
	}

	void reloadPreviousBall()
	{
		m_Balls.at(m_CurrentBallIndex)->setPosition(m_StartingBallPosition);
	}

	void leftMouseClick();
	void leftMouseRelease();

	void update(sf::Vector2f mousePosition);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

