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

SlingShot::SlingShot(float maxDragDistance, float maxImpulse, sf::Vector2f startingBallPosition)
{
	m_MaxImpulse = converter::metersToPixels(maxImpulse);
	m_MaxDragDistance	= converter::metersToPixels(maxDragDistance);
	m_CurrentLoadedBall = nullptr;
	m_StartingBallPosition = startingBallPosition;
 
	m_SlingshotRect.setPosition(startingBallPosition);
	m_SlingshotRect.setSize({ 0, converter::metersToPixels(.5f) });
	m_SlingshotRect.setOrigin({ 0, m_SlingshotRect.getSize().y/2.0f });
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

	direction = direction.normalized();

	float impulse = converter::pixelsToMeters(m_MaxImpulse * std::pow(distance / m_MaxDragDistance, 2.0f));

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
	m_CurrentMousePosition = mousePosition;
	
	bool isDragWithinRange = (m_DragMousePosition - m_StartingBallPosition).length() < m_MaxDragDistance;
	bool isDragValid = m_DragMousePosition != m_CurrentMousePosition &&
		m_CurrentMousePosition.x  < m_DragMousePosition.x &&
		m_StartingBallPosition.x  > m_DragMousePosition.x - 25;

	if (isDragging && isDragValid && isDragWithinRange)
	{
		#ifdef _DEBUG
		std::cout << std::format("\nm_StartingBallPosition.x: {}, m_DragMousePosition.x: {}, distance between start pos and drag: {}, max drag distance: {}", m_StartingBallPosition.x, m_DragMousePosition.x, (m_DragMousePosition - m_StartingBallPosition).length(), m_MaxDragDistance);
		#endif // _DEBUG


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
	if (!isDragging)
	{
		return;
	}
	target.draw(m_SlingshotRect);
}
