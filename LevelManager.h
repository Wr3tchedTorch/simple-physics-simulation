#pragma once
#include "BodyModelSerializer.h"
#include "BodyModelParser.h"
#include "BodyModelBlueprint.h"
#include <memory>
#include "PhysicsEngine.h"

class LevelManager
{
private:
	static const int LEVEL_COUNT = 5;

	std::shared_ptr<PhysicsEngine> m_PhysicsEngine;

	BodyModelSerializer m_BodyModelSerializer;
	BodyModelParser m_BodyModelParser;

	int m_CurrentLevel = 1;

public:
	LevelManager(std::shared_ptr<PhysicsEngine> physicsEngine)
	{
		m_PhysicsEngine = physicsEngine;
	}

	void nextLevel();

	void saveChangesToLevel();

	void saveBodyModelToLevel(BodyModelBlueprint model)
	{
		m_BodyModelSerializer.serializeModel(model);
	}

	void loadLevel(int index);
};

