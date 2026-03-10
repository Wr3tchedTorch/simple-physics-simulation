#pragma once
#include <string>
#include <SFML/Graphics/Color.hpp>
#include <id.h>
#include <math_functions.h>

struct BodyModelBlueprint
{	
	std::string m_Type;
	sf::Color m_Color;
	float m_MaterialDamageMultiplier;
	float m_MaxHealth;

	b2Vec2 m_Position;
	b2Vec2 m_Size;
};