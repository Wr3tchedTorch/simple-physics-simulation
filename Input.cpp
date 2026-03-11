#include "GameEngine.h"
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "LevelManager.h"
#include <iostream>

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
			if (GameEngine::IsEditMode && mouse->button == sf::Mouse::Button::Right)
			{
				m_PhysicsEngine->destroyBodyAtLocation(mousePosition);
				LevelManager::HasPendingChangesToLevel = true;
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
				if (keyPressed->scancode == sf::Keyboard::Scancode::Grave)
				{
					GameEngine::IsEditMode = false;
					m_PhysicsEngine->resumeSimulation();
				}
				else if ((m_LastPressedKey == sf::Keyboard::Scancode::LControl || m_LastPressedKey == sf::Keyboard::Scancode::RControl) && keyPressed->scancode == sf::Keyboard::Scancode::S)
				{
					m_LevelManager.saveChangesToLevel();
					LevelManager::HasPendingChangesToLevel = false;
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::N)
				{
					m_BoxFactory.nextBoxType();
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Left)
				{
					bool pressedShift = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift);
					m_BoxFactory.increaseSizeX(pressedShift ? -1 : -20);
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Right)
				{
					bool pressedShift = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift);
					m_BoxFactory.increaseSizeX(pressedShift ? 1 : 20);
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Down)
				{
					bool pressedShift = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift);
					m_BoxFactory.increaseSizeY(pressedShift ? -1 : -20);
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Up)
				{
					bool pressedShift = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift);
					m_BoxFactory.increaseSizeY(pressedShift ? 1 : 20);
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::D)
				{
					bool pressedShift = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift);
					m_BoxFactory.moveX(pressedShift ? 1 : 20);
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::A)
				{
					bool pressedShift = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift);
					m_BoxFactory.moveX(pressedShift ? -1 : -20);
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::S)
				{
					bool pressedShift = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift);
					m_BoxFactory.moveY(pressedShift? 1 : 20);
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::W)
				{
					bool pressedShift = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift);
					m_BoxFactory.moveY(pressedShift ? -1 : -20);
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::R)
				{
					m_BoxFactory.rotate(sf::degrees(15).asRadians());
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Enter)
				{
					m_BoxFactory.createBox();
					LevelManager::HasPendingChangesToLevel = true;
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Tab)
				{
					m_PhysicsEngine->switchSimulationState();
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Delete)
				{
					m_PhysicsEngine->clearWorld();
					spawnGround();
					LevelManager::HasPendingChangesToLevel = true;
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num1)
				{
					m_LevelManager.loadLevel(1);
					GameEngine::Score = 0;
					for (auto& ball : m_Balls)
					{
						ball->clearTrail();
					}
					spawnGround();
					LevelManager::HasPendingChangesToLevel = false;
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num2)
				{
					m_LevelManager.loadLevel(2);
					GameEngine::Score = 0;
					for (auto& ball : m_Balls)
					{
						ball->clearTrail();
					}

					spawnGround();
					LevelManager::HasPendingChangesToLevel = false;
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num3)
				{
					m_LevelManager.loadLevel(3);
					GameEngine::Score = 0;
					for (auto& ball : m_Balls)
					{
						ball->clearTrail();
					}
					spawnGround();
					LevelManager::HasPendingChangesToLevel = false;
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num4)
				{
					m_LevelManager.loadLevel(4);
					GameEngine::Score = 0;
					for (auto& ball : m_Balls)
					{
						ball->clearTrail();
					}
					spawnGround();
					LevelManager::HasPendingChangesToLevel = false;
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num5)
				{
					m_LevelManager.loadLevel(5);
					GameEngine::Score = 0;
					for (auto& ball : m_Balls)
					{
						ball->clearTrail();
					}
					spawnGround();
					LevelManager::HasPendingChangesToLevel = false;
				}
			}
			else if (keyPressed->scancode == sf::Keyboard::Scancode::Grave)
			{
				GameEngine::IsEditMode = true;
				m_PhysicsEngine->stopSimulation();
			}

			if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
			{
				m_Window.close();
			}

			m_LastPressedKey = keyPressed->scancode;
		}

	}
}