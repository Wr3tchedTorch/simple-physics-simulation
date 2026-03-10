#pragma once
#include <SFML/System/Vector2.hpp>
#include "Ball.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class SlingShot : public sf::Drawable
{
private:
	sf::Vector2f m_DragMousePosition;
	sf::Vector2f m_CurrentMousePosition;
	float m_MaxDragDistance;
	float m_MaxImpulse;

	bool isDragging = false;

	sf::Vector2f m_LaunchDirection;
	sf::Vector2f m_StartingBallPosition;

	sf::RectangleShape m_SlingshotRect;
	Ball* m_CurrentLoadedBall;

public:
	SlingShot(float maxDragDistance, float maxImpulse, sf::Vector2f startingBallPosition);

	void setAmmo(Ball& ball)
	{
		m_CurrentLoadedBall = &ball;
		m_CurrentLoadedBall->setPosition(m_StartingBallPosition);
	}

	void leftMouseClick();
	void leftMouseRelease();

	void update(sf::Vector2f mousePosition);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

