#include "GameEngine.h"
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/System/Angle.hpp>
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
			sf::Vector2f mousePosition = m_Window.mapPixelToCoords(sf::Mouse::getPosition(m_Window), m_GameView);
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
				if (m_LastPressedKey == sf::Keyboard::Scancode::LControl && keyPressed->scancode == sf::Keyboard::Scancode::S)
				{
					m_LevelManager.saveChangesToLevel();
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::N)
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
					m_BoxFactory.moveX(10);
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::A)
				{
					m_BoxFactory.moveX(-10);
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::S)
				{
					m_BoxFactory.moveY(10);
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::W)
				{
					m_BoxFactory.moveY(-10);
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::R)
				{
					m_BoxFactory.rotate(sf::degrees(15).asRadians());
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Enter)
				{					
					m_BoxFactory.createBox();
				}	
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Tab)
				{
					m_PhysicsEngine->switchSimulationState();
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num1)
				{
					m_LevelManager.loadLevel(1);
					m_Ball.clearTrail();
					spawnGround();
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num2)
				{
					m_LevelManager.loadLevel(2);
					m_Ball.clearTrail();
					spawnGround();
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num3)
				{
					m_LevelManager.loadLevel(3);
					m_Ball.clearTrail();
					spawnGround();
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num4)
				{
					m_LevelManager.loadLevel(4);
					m_Ball.clearTrail();
					spawnGround();
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num5)
				{
					m_LevelManager.loadLevel(5);
					m_Ball.clearTrail();
					spawnGround();
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
			m_LastPressedKey = keyPressed->scancode;
		}

	}
}