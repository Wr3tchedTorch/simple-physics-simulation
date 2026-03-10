#pragma once
#include <vector>
#include "BodyModelBlueprint.h"
#include <string>

class BodyModelParser
{
private:
	int m_CurrentLevel = 0;
	int m_LevelCount;

	std::vector<BodyModelBlueprint> m_Blueprints;
	std::string m_CurrentLevelContent;
	
public:
	void loadNextLevel();
	void setLevelCount(int count)
	{
		m_LevelCount = count;
	}
	std::vector<BodyModelBlueprint> getBlueprints();
};

