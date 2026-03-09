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
	m_MaxImpulse	    = converter::metersToPixels(maxImpulse);
	m_MaxDragDistance   = converter::metersToPixels(maxDragDistance);
	m_CurrentLoadedBall = nullptr;

	m_SlingshotRect.setPosition(m_DragMousePosition);
	m_SlingshotRect.setSize({ 1, converter::metersToPixels(.5f) });
	m_SlingshotRect.setOrigin({ 0, m_SlingshotRect.getSize().y / 2.0f });	
}

void SlingShot::leftMouseClick()
{
	isDragging = true;
	m_DragMousePosition = m_CurrentMousePosition;
}

void SlingShot::leftMouseRelease()
{
	sf::Vector2f direction = m_CurrentMousePosition - m_DragMousePosition;
	float distance = std::max(direction.length(), m_MaxDragDistance);

	direction = direction.normalized();

	float impulse = converter::pixelsToMeters(m_MaxImpulse * std::sqrt(distance / m_MaxDragDistance));

	m_CurrentLoadedBall->launch({ m_StartingBallPosition.x, m_StartingBallPosition.y }, { direction.x, direction.y }, impulse);
	isDragging = false;

	#ifdef _DEBUG
	std::cout << std::format("Launching ball: Impulse ({}); Direction (x: {}, y: {}); Distance ({})", impulse, direction.x, direction.y, distance);
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

		direction.y -= 20.0f;
		m_SlingshotRect.setRotation(direction.angle());
		m_SlingshotRect.setSize({
			distance,
			m_SlingshotRect.getSize().y
		});
	}
}

void SlingShot::draw(sf::RenderTarget& target, sf::RenderStates states) const
{	
	target.draw(m_SlingshotRect);
}
