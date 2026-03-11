#include <SFML/Graphics/Drawable.hpp>
#include "BodyModel.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>
#include "BodyModelBlueprint.h"
#include "PhysicsEngine.h"
#include <SFML/Graphics/Rect.hpp>
#include <string>
#include <SFML/Graphics/Transform.hpp>

#pragma once
class BoxFactory : public sf::Drawable
{
private:
	int m_CurrentBoxIndex;
	std::vector<BodyModel> m_BoxTypes;

	sf::RectangleShape m_Sprite;
	std::shared_ptr<PhysicsEngine> m_PhysicsEngine;

	sf::FloatRect m_WorldBounds;

public:
	BoxFactory(std::shared_ptr<PhysicsEngine> physicsEngine);

	void setGlobalBounds(sf::FloatRect worldBounds)
	{
		m_WorldBounds = worldBounds;
	}

	float getBoxRotationRadians()
	{
		return m_Sprite.getRotation().asRadians();
	}

	sf::FloatRect getBoxRect()
	{
		return {
			m_Sprite.getPosition(),
			m_Sprite.getSize()
		};
	}

	sf::Transform getBoxTransform()
	{
		return m_Sprite.getTransform();
	}

	std::string getBoxMaterialName()
	{
		return m_BoxTypes[m_CurrentBoxIndex].m_MaterialName;
	}

	void nextBoxType();
	void increaseSizeX(float amount);
	void increaseSizeY(float amount);
	void moveX(float amount);
	void moveY(float amount);
	void rotate(float radians);
	BodyModelBlueprint createBox();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

