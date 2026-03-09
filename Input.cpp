#include "GameEngine.h"
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <math_functions.h>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Angle.hpp>
#include "Converter.h"
#include "BodyModel.h"
#include <random>
#include <SFML/Window/Keyboard.hpp>

void GameEngine::input()
{
	while (auto event = m_Window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{			
			m_Window.close();
		}
		if (auto mouse = event->getIf<sf::Event::MouseButtonPressed>())
		{
			sf::Vector2f mousePosition = m_Window.mapPixelToCoords(sf::Mouse::getPosition(m_Window));
			if (mouse->button == sf::Mouse::Button::Left)
			{				
				m_SlingShot.leftMouseClick();

				sf::Angle angle = sf::degrees(45);
				b2Rot rot = b2MakeRot(angle.asRadians());

				BodyModel model;

				static std::mt19937 rng(std::random_device{}());
				static std::uniform_int_distribution<int> dist(0, 255);

				int red   = dist(rng);
				int green = dist(rng);
				int blue  = dist(rng);

				model.m_Color = sf::Color(red, green, blue);
				m_PhysicsEngine.spawnBodyAtLocation({ converter::pixelsToMeters(mousePosition.x), converter::pixelsToMeters(mousePosition.y) }, {1, 1}, rot, model);
			}
			if (mouse->button == sf::Mouse::Button::Right)
			{
				m_PhysicsEngine.destroyBodyAtLocation(mousePosition);
			}			
		}
		if (auto mouse = event->getIf<sf::Event::MouseButtonReleased>())
		{
			if (mouse->button == sf::Mouse::Button::Left)
			{
				m_SlingShot.leftMouseRelease();
			}
		}
	}
}