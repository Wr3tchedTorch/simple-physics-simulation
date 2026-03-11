#pragma once
#include <string>
#include "BodyModelBlueprint.h"
#include <vector>

class BodyModelSerializer
{
private:
	std::string m_FilePath;
	std::string m_LevelChanges;

	bool m_ChangesSaved = false;

public:
	void loadLevel(std::string filepath)
	{
		m_FilePath     = filepath;
		m_ChangesSaved = true;
	}

	void clearLevel() 
	{
		m_LevelChanges = "";
		m_ChangesSaved = true;
	}

	void serializeModel(BodyModelBlueprint model);

	void serializeWorld(std::vector<BodyModelBlueprint> blueprints);
	void saveChanges();
};

