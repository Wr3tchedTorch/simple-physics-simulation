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

void GameEngine::draw()
{
	m_Window.clear();

	m_PhysicsEngine.forEachBody([this](b2BodyId id, const BodyModel& body) -> void
		{
			sf::RectangleShape sprite;
			sprite.setFillColor(body.m_Color);

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

	m_Window.draw(m_Ball);
	m_Window.draw(m_SlingShot);

	m_Window.display();
}