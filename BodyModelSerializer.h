#pragma once
#include <string>
#include "BodyModelBlueprint.h"
#include <vector>

class BodyModelSerializer
{
private:
	std::string m_FilePath;
	std::string m_LevelChanges;

public:
	void loadLevel(std::string filepath)
	{
		m_FilePath = filepath;
	}

	void clearLevel() 
	{
		m_LevelChanges = "";
	}

	void serializeModel(BodyModelBlueprint model);

	void serializeWorld(std::vector<BodyModelBlueprint> blueprints);
	void saveChanges();
};

