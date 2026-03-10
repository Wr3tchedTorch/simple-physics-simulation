#pragma once
#include <string>
#include <SFML/Graphics/Color.hpp>
#include <id.h>
#include <math_functions.h>

struct BodyModel
{	
	std::string m_Type = "box";
	sf::Color m_Color;
	b2BodyId  m_Id;
	float m_MaterialDamageMultiplier = 1.0f;
	float m_MaxHealth = 100.0f;

	b2Vec2 m_Position;
	b2Vec2 m_Size;
};