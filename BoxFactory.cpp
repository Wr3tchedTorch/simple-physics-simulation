#include "BoxFactory.h"
#include <memory>
#include "PhysicsEngine.h"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include "Converter.h"
#include <math_functions.h>
#include <SFML/Graphics/Color.hpp>
#include "BodyModel.h"
#include "GameEngine.h"
#include "BodyModelBlueprint.h"

BoxFactory::BoxFactory(std::shared_ptr<PhysicsEngine> physicsEngine)
{	
	m_Sprite.setSize({ 50, 50 });
	m_Sprite.setPosition({ 300, 300 });
	m_Sprite.setOrigin({ 25, 25 });
	m_Sprite.setOutlineColor(sf::Color::White);
	m_Sprite.setOutlineThickness(3);

	m_PhysicsEngine = physicsEngine;

	m_BoxTypes.emplace_back(BodyModel{
		.m_Color  = sf::Color(171, 253, 255),
		.m_MaterialDamageMultiplier = 3.0f,
		.m_Health = 75,
		.m_MaxHealth = 75
	});
	m_BoxTypes.emplace_back(BodyModel{
		.m_Color = sf::Color(204, 153, 102),
		.m_MaterialDamageMultiplier = 1.5f,
		.m_Health = 225,
		.m_MaxHealth = 225
	});
	m_BoxTypes.emplace_back(BodyModel{
		.m_Color = sf::Color(138, 138, 138),
		.m_MaterialDamageMultiplier = 1.0f,
		.m_Health = 375,
		.m_MaxHealth = 375
	});

	m_Sprite.setFillColor(m_BoxTypes[m_CurrentBoxIndex].m_Color);
}

void BoxFactory::nextBoxType()
{
	m_CurrentBoxIndex++;

	if (m_CurrentBoxIndex >= m_BoxTypes.size())
	{
		m_CurrentBoxIndex = 0;
	}

	sf::Color ghostColor = m_BoxTypes[m_CurrentBoxIndex].m_Color;
	ghostColor.a = 127;
	m_Sprite.setFillColor(ghostColor);
}

void BoxFactory::increaseSizeX(float amount)
{
	sf::Vector2f toSize = m_Sprite.getSize();
	if (toSize.x + amount < 0)
	{
		return;
	}

	toSize.x += amount;
	m_Sprite.setSize(toSize);
	m_Sprite.setOrigin({ toSize.x / 2.0f, toSize.y / 2.0f });
}

void BoxFactory::increaseSizeY(float amount)
{
	sf::Vector2f toSize = m_Sprite.getSize();
	if (toSize.y + amount < 0)
	{
		return;
	}

	toSize.y += amount;
	m_Sprite.setSize(toSize);
	m_Sprite.setOrigin({ toSize.x / 2.0f, toSize.y / 2.0f });
}

void BoxFactory::moveX(float amount)
{
	if (m_Sprite.getPosition().x + amount < 0 ||
		m_Sprite.getPosition().x + amount > GameEngine::Resolution.x)
	{
		return;
	}

	m_Sprite.move({ amount, 0.0f });
}

void BoxFactory::moveY(float amount)
{
	if (m_Sprite.getPosition().y + amount < 0 ||
		m_Sprite.getPosition().y + amount > GameEngine::Resolution.y)
	{
		return;
	}

	m_Sprite.move({0, amount});
}

void BoxFactory::rotate(float radians)
{
	m_Sprite.rotate(sf::radians(radians));
}

BodyModelBlueprint BoxFactory::createBox()
{
	b2Vec2 locationInMeters =
	{
		converter::pixelsToMeters(m_Sprite.getPosition().x),
		converter::pixelsToMeters(m_Sprite.getPosition().y)
	};

	b2Vec2 sizeInMeters =
	{
		converter::pixelsToMeters(m_Sprite.getSize().x),
		converter::pixelsToMeters(m_Sprite.getSize().y)
	};

	b2Rot angle = b2MakeRot(m_Sprite.getRotation().asRadians());

	BodyModel model = m_BoxTypes[m_CurrentBoxIndex];
	model.m_StartingPosition = locationInMeters;
	model.m_Size = sizeInMeters;

	m_PhysicsEngine->spawnBodyAtLocation(locationInMeters, sizeInMeters, angle, model);

	BodyModelBlueprint modelBlueprint;
	modelBlueprint.m_Color = m_BoxTypes[m_CurrentBoxIndex].m_Color;
	modelBlueprint.m_Type  = m_BoxTypes[m_CurrentBoxIndex].m_Type;
	modelBlueprint.m_MaxHealth = m_BoxTypes[m_CurrentBoxIndex].m_MaxHealth;
	modelBlueprint.m_MaterialDamageMultiplier = m_BoxTypes[m_CurrentBoxIndex].m_MaterialDamageMultiplier;
	modelBlueprint.m_Position = locationInMeters;
	modelBlueprint.m_Size = sizeInMeters;

	return modelBlueprint;
}

void BoxFactory::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Sprite, states);
}


