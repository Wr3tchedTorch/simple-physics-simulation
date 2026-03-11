#pragma once
#include <vector>
#include "BodyModelBlueprint.h"
#include <string>

class BodyModelParser
{
private:
	std::vector<BodyModelBlueprint> m_Blueprints;
	std::string m_CurrentLevelContent;
	
public:
	void loadLevel(int level);

	std::string getCurrentLevelContent()
	{
		return m_CurrentLevelContent;
	}
	
	std::vector<BodyModelBlueprint> getBlueprints()
	{
		return m_Blueprints;
	}
};

