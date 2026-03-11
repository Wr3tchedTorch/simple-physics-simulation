#include "GameEngine.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <box2d.h>
#include <types.h>
#include <id.h>
#include "BodyModel.h"
#include "Converter.h"
#include <iostream>

void GameEngine::update(float delta)
{
	m_PhysicsEngine->update(delta);

	b2ContactEvents contactEvents = b2World_GetContactEvents(m_PhysicsEngine->getWorld());
	for (int i = 0; i < contactEvents.hitCount; ++i)
	{		
		b2ContactHitEvent* hitEvent = contactEvents.hitEvents + i;

		b2ShapeId shapeA = hitEvent->shapeIdA;
		b2ShapeId shapeB = hitEvent->shapeIdB;

		b2BodyId bodyA = b2Shape_GetBody(shapeA);
		b2BodyId bodyB = b2Shape_GetBody(shapeB);

		if (!b2Body_IsValid(bodyA) || !b2Body_IsValid(bodyB))
		{
			return;
		}

		bool isStaticBodyCollision = b2Body_GetType(bodyA) == b2_staticBody ||
									 b2Body_GetType(bodyB) == b2_staticBody;

		bool isBallPresent = m_SlingShot.isBall(bodyA) ||
							 m_SlingShot.isBall(bodyB);

		if (isStaticBodyCollision && !isBallPresent)
		{			
			b2BodyId staticBody = b2Body_GetType(bodyB) == b2_staticBody ? bodyB : bodyA;
			b2BodyId box		= B2_ID_EQUALS(staticBody, bodyA) ? bodyB : bodyA;

			BodyModel* model = static_cast<BodyModel*>(b2Body_GetUserData(box));

			model->m_Health -= 3 * hitEvent->approachSpeed;
			if (model->m_Health <= 0)
			{
				m_PhysicsEngine->destroyBodyById(box);
				GameEngine::Score += 100;
			}

			return;
		}

		if (!isBallPresent)
		{
			BodyModel* modelA = static_cast<BodyModel*>(b2Body_GetUserData(bodyA));
			BodyModel* modelB = static_cast<BodyModel*>(b2Body_GetUserData(bodyB));

			modelA->m_Health -= 5 * hitEvent->approachSpeed;
			modelB->m_Health -= 5 * hitEvent->approachSpeed;

			if (modelA->m_Health <= 0)
			{
				m_PhysicsEngine->destroyBodyById(bodyA);
				GameEngine::Score += 100;
			}
			if (modelB->m_Health <= 0)
			{
				m_PhysicsEngine->destroyBodyById(bodyB);
				GameEngine::Score += 100;
			}

			return;
		}

		if (isStaticBodyCollision)
		{
			return;
		}

		b2BodyId   otherBody = m_SlingShot.isBall(bodyA) ? bodyB : bodyA;
		BodyModel* model = static_cast<BodyModel*>(b2Body_GetUserData(otherBody));

		if (model->m_Type == "box")
		{
#ifdef _DEBUG
			std::cout << hitEvent->approachSpeed << "\n";
#endif // _DEBUG


			if (m_Balls.at(0)->applyDamage(model, hitEvent->approachSpeed))
			{
				m_PhysicsEngine->destroyBodyById(otherBody);
				GameEngine::Score += 100;
			}
		}
	}

	sf::Vector2f mousePosition = m_Window.mapPixelToCoords(sf::Mouse::getPosition(m_Window), m_GameView);
	m_SlingShot.update(mousePosition);

	for (auto& ball : m_Balls)
	{
		ball->update();
	}

	if (m_SlingShot.allBallsStopped())
	{		
		for (int i = 0; i < m_Balls.size(); i++)
		{
			auto& ball = m_Balls[i];

			ball->clearTrail();

			ball->setPosition({
				-converter::metersToPixels(7.0f + i * 2),
				m_GameView.getCenter().y + m_GameView.getSize().y / 2.0f - converter::metersToPixels(5) - ball->getSprite().getRadius() / 2.0f
			});
			ball->reset();
		}
		m_SlingShot.reload();

		if (!GameEngine::IsEditMode)
		{					
			m_LevelManager.nextLevel();
			GameEngine::Score = 0;

			spawnGround();
		}
	}
}