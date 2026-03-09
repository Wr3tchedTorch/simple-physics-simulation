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

SlingShot::SlingShot(float maxDragDistance, float maxImpulse)
{
	m_MaxImpulse = converter::metersToPixels(maxImpulse);
	m_MaxDragDistance = converter::metersToPixels(maxDragDistance);
	m_CurrentLoadedBall = nullptr;

	m_SlingshotRect.setPosition(m_DragMousePosition);
	m_SlingshotRect.setSize({ 0, converter::metersToPixels(.5f) });
	m_SlingshotRect.setOrigin({ 0, m_SlingshotRect.getSize().y/2.0f });
}

void SlingShot::leftMouseClick()
{
	m_SlingshotRect.setSize({ 0, m_SlingshotRect.getSize().y });
	isDragging = true;
	m_DragMousePosition = m_CurrentMousePosition;
	m_CurrentLoadedBall->reset();
}

void SlingShot::leftMouseRelease()
{
	if (m_DragMousePosition == m_CurrentMousePosition || m_CurrentMousePosition.x > m_DragMousePosition.x)
	{
		isDragging = false;		
		return;
	}

	sf::Vector2f direction = m_DragMousePosition - m_CurrentMousePosition;
	float distance = std::min(direction.length(), m_MaxDragDistance);

	direction = direction.normalized();

	float impulse = converter::pixelsToMeters(m_MaxImpulse * std::sqrt(distance / m_MaxDragDistance));

	m_CurrentLoadedBall->launch({ converter::pixelsToMeters(m_StartingBallPosition.x), converter::pixelsToMeters(m_StartingBallPosition.y) }, { direction.x, direction.y }, impulse);
	isDragging = false;

	#ifdef _DEBUG
	std::cout << std::format("\nLaunching ball: Impulse ({}); Direction (x: {}, y: {}); Distance ({}), Starting Position (x: {}, y: {})", impulse, direction.x, direction.y, distance, m_StartingBallPosition.x, m_StartingBallPosition.y);
	#endif // _DEBUG
}

void SlingShot::update(sf::Vector2f mousePosition)
{
	m_CurrentMousePosition = mousePosition;
	if (isDragging && m_DragMousePosition != m_CurrentMousePosition && m_CurrentMousePosition.x < m_DragMousePosition.x)
	{
		sf::Vector2f direction = m_CurrentMousePosition - m_DragMousePosition;
		float distance = std::min(direction.length(), m_MaxDragDistance);

		m_SlingshotRect.setPosition(m_DragMousePosition);

		m_StartingBallPosition = m_DragMousePosition + direction.normalized() * distance;
		m_CurrentLoadedBall->setPosition(m_StartingBallPosition);

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
