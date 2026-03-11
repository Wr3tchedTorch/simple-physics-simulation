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

	void nextBoxType();
	void increaseSizeX(float amount);
	void increaseSizeY(float amount);
	void moveX(float amount);
	void moveY(float amount);
	void rotate(float radians);
	BodyModelBlueprint createBox();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

