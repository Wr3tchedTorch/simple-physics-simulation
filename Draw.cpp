#include "GameEngine.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <box2d.h>
#include <id.h>
#include "Converter.h"
#include <math_functions.h>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <collision.h>
#include "BodyModel.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <format>
#include "LevelManager.h"

void GameEngine::draw()
{
	m_Window.clear();

	m_Window.setView(m_GameView);
	m_PhysicsEngine->forEachBody([this](b2BodyId id, const BodyModel& body) -> void
		{
			if (!b2Body_IsValid(id))
			{
				return;
			}

			sf::RectangleShape sprite;
			sprite.setFillColor(body.m_Color);			
			
			if (body.m_Type == "box")
			{
				float percentage = body.m_Health / body.m_MaxHealth;
				float fadeColor = 255 * percentage;
				sprite.setOutlineColor(sf::Color(255, fadeColor, fadeColor));
				sprite.setOutlineThickness(3);
			}

			b2Transform transformInMeters = b2Body_GetTransform(id);
			b2Rot bodyRot = b2Body_GetRotation(id);

			sf::Vector2f position({
					converter::metersToPixels(transformInMeters.p.x),
					converter::metersToPixels(transformInMeters.p.y)
				});

			float radians = b2Rot_GetAngle(transformInMeters.q);
			sf::Angle rotation(sf::radians(radians));

			b2ShapeId shape;

			b2Body_GetShapes(id, &shape, 1);

			b2Polygon polygon = b2Shape_GetPolygon(shape);
			float width = polygon.vertices[1].x * 2.0f;
			float height = polygon.vertices[2].y * 2.0f;

			float widthInPixel = converter::metersToPixels(width);
			float heightInPixel = converter::metersToPixels(height);

			sprite.setSize({
				widthInPixel,
				heightInPixel
				});
			sprite.setPosition(position);
			sprite.setRotation(rotation);
			sprite.setOrigin({ widthInPixel / 2.0f, heightInPixel / 2.0f });

			m_Window.draw(sprite);
		});

	if (GameEngine::IsEditMode)
	{
		m_Window.draw(m_BoxFactory);
	}
	m_Window.draw(m_SlingShot);
	m_Window.draw(m_Ball);

	m_Window.setView(m_HUDView);

	if (IsEditMode)
	{
		m_HUD.drawEditorModeHUD(
			m_Window,
			m_HUDView,
			m_GameView,
			m_BoxFactory.getBoxMaterialName(),
			m_BoxFactory.getBoxRotationRadians(),
			m_BoxFactory.getBoxRect(),
			std::format("Level {}", m_LevelManager.getCurrentLevelIndex()),
			LevelManager::HasPendingChangesToLevel
		);
	}
	else
	{
		m_HUD.drawGameHUD(
			m_Window,
			m_HUDView,
			std::format("Level {}", m_LevelManager.getCurrentLevelIndex())
		);
	}

	m_Window.display();
}