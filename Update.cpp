#include "GameEngine.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <box2d.h>
#include <types.h>
#include <id.h>
#include "BodyModel.h"

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


		bool isBallPresent = B2_ID_EQUALS(m_Ball.getBodyId(), bodyA) ||
							 B2_ID_EQUALS(m_Ball.getBodyId(), bodyB);

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

		b2BodyId   otherBody = B2_ID_EQUALS(m_Ball.getBodyId(), bodyA) ? bodyB : bodyA;
		BodyModel* model = static_cast<BodyModel*>(b2Body_GetUserData(otherBody));

		if (model->m_Type == "box")
		{
			if (m_Ball.applyDamage(model, hitEvent->approachSpeed))
			{
				m_PhysicsEngine->destroyBodyById(otherBody);
				GameEngine::Score += 100;
			}
		}
	}

	sf::Vector2f mousePosition = m_Window.mapPixelToCoords(sf::Mouse::getPosition(m_Window), m_GameView);
	m_SlingShot.update(mousePosition);
	m_Ball.update();

	if (m_Ball.getLaunchCount() >= 3)
	{
		m_LevelManager.nextLevel();
		GameEngine::Score = 0;

		m_Ball.resetLaunchCount();
		spawnGround();

	}
}