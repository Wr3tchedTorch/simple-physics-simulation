#include "BodyModelParser.h"
#include <format>
#include <fstream>
#include <string>
#include "ObjectSerializationTags.h"
#include <sstream>
#include <vector>
#include <SFML/Graphics/Color.hpp>
#include "BodyModelBlueprint.h"

void BodyModelParser::loadLevel(int level)
{
	m_CurrentLevel++;
	std::ifstream inputFile(std::format("levels/level{}.DAT", level));	

	if (inputFile.is_open())
	{
		std::string line;			

		BodyModelBlueprint model;
		while (std::getline(inputFile, line))
		{
			m_CurrentLevelContent += line + "\n";

			if (line.find(ObjectSerializationTags::START_OBJECT) != line.npos)
			{
				model = BodyModelBlueprint();
				continue;
			}

			if (line.find(ObjectSerializationTags::START_TYPE) != line.npos)
			{
				int endPos = line.size() - ObjectSerializationTags::END_TYPE.size() - ObjectSerializationTags::START_TYPE.size();
				std::string type = line.substr(ObjectSerializationTags::START_TYPE.size(), endPos);

				model.m_Type = type;
				continue;
			}

			if (line.find(ObjectSerializationTags::START_POSITION) != line.npos)
			{
				int endPos = line.size() - ObjectSerializationTags::END_POSITION.size() - ObjectSerializationTags::START_POSITION.size();
				std::string data = line.substr(ObjectSerializationTags::START_POSITION.size(), endPos);

				std::stringstream stream(data);
				std::vector<float> xy;
				std::string splitData;
				while (std::getline(stream, splitData, ','))
				{
					xy.push_back(std::stof(splitData));
				}

				b2Vec2 position({ xy[0], xy[1] });
				model.m_Position = position;
				continue;
			}

			if (line.find(ObjectSerializationTags::START_SIZE) != line.npos)
			{
				int endPos = line.size() - ObjectSerializationTags::END_SIZE.size() - ObjectSerializationTags::START_SIZE.size();
				std::string data = line.substr(ObjectSerializationTags::START_SIZE.size(), endPos);

				std::stringstream stream(data);
				std::vector<float> xy;
				std::string splitData;
				while (std::getline(stream, splitData, ','))
				{
					xy.push_back(std::stof(splitData));
				}

				b2Vec2 size({ xy[0], xy[1] });
				model.m_Size = size;
				continue;
			}

			if (line.find(ObjectSerializationTags::START_MAX_HEALTH) != line.npos)
			{
				int endPos = line.size() - ObjectSerializationTags::END_MAX_HEALTH.size() - ObjectSerializationTags::START_MAX_HEALTH.size();
				std::string type = line.substr(ObjectSerializationTags::START_MAX_HEALTH.size(), endPos);

				model.m_MaxHealth = std::stof(type);
				continue;
			}

			if (line.find(ObjectSerializationTags::START_COLOR) != line.npos)
			{
				int endPos = line.size() - ObjectSerializationTags::END_COLOR.size() - ObjectSerializationTags::START_COLOR.size();
				std::string data = line.substr(ObjectSerializationTags::START_COLOR.size(), endPos);

				std::stringstream stream(data);
				std::vector<int> rgba;
				std::string colorData;
				while (std::getline(stream, colorData, ','))
				{
					rgba.push_back(std::stoi(colorData));
				}

				sf::Color color(rgba[0], rgba[1], rgba[2], rgba[3]);
				model.m_Color = color;
				continue;
			}

			if (line.find(ObjectSerializationTags::START_MATERIAL_DAMAGE_MULTIPLIER) != line.npos)
			{
				int endPos = line.size() - ObjectSerializationTags::END_MATERIAL_DAMAGE_MULTIPLIER.size() - ObjectSerializationTags::START_MATERIAL_DAMAGE_MULTIPLIER.size();
				std::string type = line.substr(ObjectSerializationTags::START_MATERIAL_DAMAGE_MULTIPLIER.size(), endPos);

				model.m_MaterialDamageMultiplier = std::stof(type);
				continue;
			}

			if (line.find(ObjectSerializationTags::END_OBJECT) != line.npos)
			{
				m_Blueprints.push_back(model);
			}
		}
	}

	m_CurrentLevel = level;
}
