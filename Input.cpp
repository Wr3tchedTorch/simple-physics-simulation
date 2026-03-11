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
			}
			if (mouse->button == sf::Mouse::Button::Right)
			{
				m_PhysicsEngine->destroyBodyAtLocation(mousePosition);
			}
		}
		if (auto mouse = event->getIf<sf::Event::MouseButtonReleased>())
		{
			if (mouse->button == sf::Mouse::Button::Left)
			{
				m_SlingShot.leftMouseRelease();
			}
		}
		if (auto keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			if (GameEngine::IsEditMode)
			{
				if (keyPressed->scancode == sf::Keyboard::Scancode::N)
				{
					m_BoxFactory.nextBoxType();
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Left)
				{
					m_BoxFactory.increaseSizeX(-20);
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Right)
				{
					m_BoxFactory.increaseSizeX(20);
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Down)
				{
					m_BoxFactory.increaseSizeY(-20);
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Up)
				{
					m_BoxFactory.increaseSizeY(20);
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::D)
				{
					m_BoxFactory.moveX(20);
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::A)
				{
					m_BoxFactory.moveX(-20);
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::S)
				{
					m_BoxFactory.moveY(20);
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::W)
				{
					m_BoxFactory.moveY(-20);
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::R)
				{
					m_BoxFactory.rotate(sf::degrees(90).asRadians());
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Enter)
				{					
					m_BodyModelSerializer.serializeModel(m_BoxFactory.createBox());
					m_BodyModelSerializer.saveChanges();
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::S)
				{
					// save current level
				}				
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Tab)
				{
					m_PhysicsEngine->switchSimulationState();
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num1)
				{
					m_BodyModelParser.loadLevel(1);
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num2)
				{
					m_BodyModelParser.loadLevel(2);
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num3)
				{
					m_BodyModelParser.loadLevel(3);
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num4)
				{
					m_BodyModelParser.loadLevel(4);
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num5)
				{
					m_BodyModelParser.loadLevel(5);
				}
			}
			
			if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
			{
				m_Window.close();
			}
			else if (keyPressed->scancode == sf::Keyboard::Scancode::Space)
			{
				GameEngine::IsEditMode = false;
				m_PhysicsEngine->resumeSimulation();
			}
			else if (keyPressed->scancode == sf::Keyboard::Scancode::Backslash)
			{
				GameEngine::IsEditMode = true;
				m_PhysicsEngine->stopSimulation();
			}
		}

	}
}