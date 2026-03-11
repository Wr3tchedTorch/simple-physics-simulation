#include "SlingShot.h"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include "Converter.h"
#include <cmath>
#include <iostream>
#include <format>
#include <SFML/Graphics/RectangleShape.hpp>
#include <math_functions.h>
#include "GameEngine.h"
#include <SFML/Graphics/Color.hpp>

SlingShot::SlingShot(float maxDragDistance, float maxImpulse, sf::Vector2f startingBallPosition)
{
	m_MaxImpulse = converter::metersToPixels(maxImpulse);
	m_MaxDragDistance	= converter::metersToPixels(maxDragDistance);
	m_CurrentLoadedBall = nullptr;
	m_StartingBallPosition = startingBallPosition;
 
	m_SlingshotRect.setPosition(startingBallPosition);
	m_SlingshotRect.setSize({ 0, converter::metersToPixels(.25f) });
	m_SlingshotRect.setOrigin({ 0, m_SlingshotRect.getSize().y/2.0f });	

	m_SlingshotBaseRect.setPosition(startingBallPosition);
	m_SlingshotBaseRect.setSize(
		{ 
			converter::metersToPixels(1),  
			250
		}
	);
	m_SlingshotBaseRect.setFillColor(sf::Color(153, 102, 51));

	m_SlingshotBaseRect.setOrigin({ m_SlingshotBaseRect.getSize().x / 2.0f, 0 });
}

void SlingShot::leftMouseClick()
{
	m_DragMousePosition = m_CurrentMousePosition;

	bool isDragWithinRange = (m_DragMousePosition - m_StartingBallPosition).length() < m_MaxDragDistance;
	bool isDragValid = m_StartingBallPosition.x > m_DragMousePosition.x - 25;

	if (!isDragValid || !isDragWithinRange)
	{
		isDragging = false;
		return;
	}

	m_SlingshotRect.setSize({ 0, m_SlingshotRect.getSize().y });
	isDragging = true;
	m_CurrentLoadedBall->reset();
	m_CurrentLoadedBall->setPosition(m_StartingBallPosition);
}

void SlingShot::leftMouseRelease()
{
	bool isDragWithinRange = (m_DragMousePosition - m_StartingBallPosition).length() < m_MaxDragDistance;
	bool isDragValid = m_DragMousePosition != m_CurrentMousePosition &&
					   m_CurrentMousePosition.x  < m_DragMousePosition.x &&
					   m_StartingBallPosition.x  > m_DragMousePosition.x - 25;

	if (!isDragValid || !isDragWithinRange)
	{
		isDragging = false;	
		return;
	}

	sf::Vector2f direction = m_DragMousePosition - m_CurrentMousePosition;
	float distance = std::min(direction.length(), m_MaxDragDistance);
	
	direction.y *= 1.3f;
	direction = direction.normalized();

	float impulse = converter::pixelsToMeters(m_MaxImpulse * std::sqrt(distance / m_MaxDragDistance));

	b2Vec2 startPos =
	{
		converter::pixelsToMeters(m_CurrentLoadedBall->getSprite().getPosition().x),
		converter::pixelsToMeters(m_CurrentLoadedBall->getSprite().getPosition().y)
	};

	m_CurrentLoadedBall->launch(startPos, {direction.x, direction.y}, impulse);
	isDragging = false;

	#ifdef _DEBUG
	std::cout << std::format("\nLaunching ball: Impulse ({}); Direction (x: {}, y: {}); Distance ({}), Starting Position (x: {}, y: {})", impulse, direction.x, direction.y, distance, m_StartingBallPosition.x, m_StartingBallPosition.y);
	#endif // _DEBUG
}

void SlingShot::update(sf::Vector2f mousePosition)
{
	bool isBallOutOfScreen = m_CurrentLoadedBall->getSprite().getPosition().x - m_CurrentLoadedBall->getSprite().getRadius() / 2.0f > m_GlobalBounds.position.x + m_GlobalBounds.size.x		||
							 m_CurrentLoadedBall->getSprite().getPosition().x + m_CurrentLoadedBall->getSprite().getRadius() / 2.0f < m_GlobalBounds.position.x ||
							 m_CurrentLoadedBall->getSprite().getPosition().y - m_CurrentLoadedBall->getSprite().getRadius() / 2.0f > m_GlobalBounds.size.y;
	if (isBallOutOfScreen)
	{
		m_CurrentLoadedBall->reset();
		m_CurrentLoadedBall->setPosition(m_StartingBallPosition);
	}

	m_CurrentMousePosition = mousePosition;
	
	bool isDragWithinRange = (m_DragMousePosition - m_StartingBallPosition).length() < m_MaxDragDistance;
	bool isDragValid = m_DragMousePosition != m_CurrentMousePosition &&
					   m_CurrentMousePosition.x  < m_DragMousePosition.x &&
					   m_StartingBallPosition.x  > m_DragMousePosition.x - 25;

	if (isDragging && !isDragValid)
	{
		m_CurrentLoadedBall->reset();
		m_CurrentLoadedBall->setPosition(m_StartingBallPosition);
		m_SlingshotRect.setSize({ 0, m_SlingshotRect.getSize().y });
	}

	if (isDragging && isDragValid && isDragWithinRange)
	{	
		sf::Vector2f direction = m_CurrentMousePosition - m_DragMousePosition;
		float distance = std::min(direction.length(), m_MaxDragDistance);

		m_SlingshotRect.setPosition(m_StartingBallPosition);
		m_CurrentLoadedBall->setPosition(m_StartingBallPosition + direction.normalized() * distance);

		m_SlingshotRect.setRotation(direction.angle());
		m_SlingshotRect.setSize({
			distance,
			m_SlingshotRect.getSize().y
		});		
	}
}

void SlingShot::draw(sf::RenderTarget& target, sf::RenderStates states) const
{	
	target.draw(m_SlingshotBaseRect);
	if (!isDragging)
	{
		return;
	}
	target.draw(m_SlingshotRect);
}
