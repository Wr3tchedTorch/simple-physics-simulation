#include "BodyModelParser.h"
#include <format>
#include <fstream>
#include <string>
#include "ObjectSerializationTags.h"
#include <sstream>
#include <vector>
#include <SFML/Graphics/Color.hpp>

void BodyModelParser::loadNextLevel()
{
	m_CurrentLevel++;
	std::ifstream inputFile(std::format("levels/level{}.DAT", m_CurrentLevel));

	if (inputFile.is_open())
	{
		std::string line;			

		BodyModelBlueprint model;
		while (std::getline(inputFile, line))
		{
			m_CurrentLevelContent += line + "\n";

			if (line.find(ObjectSerializationTags::START_TYPE))
			{				
				int endPos = line.size() - ObjectSerializationTags::END_TYPE.size() - ObjectSerializationTags::START_TYPE.size();
				std::string type = line.substr(ObjectSerializationTags::START_TYPE.size(), endPos);

				model.m_Type = type;
			}

			if (line.find(ObjectSerializationTags::START_COLOR))
			{
				int endPos = line.size() - ObjectSerializationTags::END_TYPE.size() - ObjectSerializationTags::START_TYPE.size();
				std::string color  = line.substr(ObjectSerializationTags::START_TYPE.size(), endPos);

				std::stringstream stream(color);
				std::vector<int> rgba;
				std::string colorData;
				while (std::getline(stream, colorData, ','))
				{
					rgba.push_back(std::stof(colorData));
				}
				
				sf::Color color(rgba[0], rgba[1], rgba[2], rgba[3]);
			}
		}
	}
}
