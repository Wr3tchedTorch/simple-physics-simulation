#include "LevelManager.h"
#include <format>
#include "BodyModel.h"
#include <math_functions.h>
#include <id.h>
#include "BodyModelBlueprint.h"

void LevelManager::nextLevel()
{
	loadLevel(m_CurrentLevel);

	m_CurrentLevel++;
	if (m_CurrentLevel > LevelManager::LEVEL_COUNT)
	{
		m_CurrentLevel = 1;
	}
}

void LevelManager::saveChangesToLevel()
{
	m_BodyModelSerializer.clearLevel();

	m_PhysicsEngine->forEachBody([this](b2BodyId id, const BodyModel& body) -> void
		{
			if (body.m_Type != "box")
			{
				return;
			}

			BodyModelBlueprint blueprint;
			blueprint.m_Color = body.m_Color;
			blueprint.m_Type = body.m_Type;
			blueprint.m_MaxHealth = body.m_MaxHealth;
			blueprint.m_MaterialDamageMultiplier = body.m_MaterialDamageMultiplier;
			blueprint.m_Position = body.m_StartingPosition;
			blueprint.m_Rotation = body.m_StartingRotation;
			blueprint.m_Size = body.m_Size;

			m_BodyModelSerializer.serializeModel(blueprint);
		});

	m_BodyModelSerializer.saveChanges();
}

void LevelManager::loadLevel(int index)
{
	m_BodyModelSerializer.clearLevel();

	m_CurrentLevel = index;

	m_BodyModelParser.loadLevel(index);

	m_BodyModelSerializer.loadLevel(std::format("levels/level{}.DAT", index));	
	m_PhysicsEngine->clearWorld();

	for (auto& body : m_BodyModelParser.getBlueprints())
	{
		BodyModel model;
		model.m_Type   = body.m_Type;
		model.m_Health = body.m_MaxHealth;
		model.m_MaxHealth = body.m_MaxHealth;
		model.m_Color	  = body.m_Color;
		model.m_MaterialDamageMultiplier = body.m_MaterialDamageMultiplier;
		model.m_StartingPosition = body.m_Position;
		model.m_Size = body.m_Size;
		model.m_StartingRotation = body.m_Rotation;

		m_PhysicsEngine->spawnBodyAtLocation(body.m_Position, body.m_Size, body.m_Rotation, model);
	}
}
