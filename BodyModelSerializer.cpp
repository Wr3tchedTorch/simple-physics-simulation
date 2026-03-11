#include "BodyModelSerializer.h"
#include <fstream>
#include <string>
#include "ObjectSerializationTags.h"
#include <format>
#include <math_functions.h>
#include "BodyModelBlueprint.h"
#include <vector>

void BodyModelSerializer::serializeModel(BodyModelBlueprint model)
{	
	std::string colorSerialized	   = std::format("{}, {}, {}, {}", model.m_Color.r, model.m_Color.g, model.m_Color.b, model.m_Color.a);
	std::string positionSerialized = std::format("{}, {}", model.m_Position.x, model.m_Position.y);
	std::string sizeSerialized	   = std::format("{}, {}", model.m_Size.x, model.m_Size.y);

	std::string serializedModel;
	serializedModel += ObjectSerializationTags::START_OBJECT + "\n";
	serializedModel += ObjectSerializationTags::START_TYPE + model.m_Type + ObjectSerializationTags::END_TYPE + "\n";
	serializedModel += ObjectSerializationTags::START_POSITION + positionSerialized + ObjectSerializationTags::END_POSITION + "\n";

	serializedModel += ObjectSerializationTags::START_SIZE +
							sizeSerialized +
						ObjectSerializationTags::END_SIZE + "\n";

	serializedModel += ObjectSerializationTags::START_ROTATION +
							std::to_string(b2Rot_GetAngle(model.m_Rotation)) +
						ObjectSerializationTags::END_ROTATION + "\n";

	serializedModel += ObjectSerializationTags::START_MAX_HEALTH +
							std::to_string(model.m_MaxHealth) +
						ObjectSerializationTags::END_MAX_HEALTH + "\n";

	serializedModel += ObjectSerializationTags::START_COLOR +
							colorSerialized +
						ObjectSerializationTags::END_COLOR + "\n";

	serializedModel += ObjectSerializationTags::START_MATERIAL_DAMAGE_MULTIPLIER +
							std::to_string(model.m_MaterialDamageMultiplier) +
						ObjectSerializationTags::END_MATERIAL_DAMAGE_MULTIPLIER + "\n";

	serializedModel += ObjectSerializationTags::END_OBJECT;

	m_LevelChanges += serializedModel + "\n\n";
	m_ChangesSaved = false;
}

void BodyModelSerializer::serializeWorld(std::vector<BodyModelBlueprint> blueprints)
{
	for (auto& blueprint : blueprints)
	{
		serializeModel(blueprint);
	}
	m_ChangesSaved = false;
}

void BodyModelSerializer::saveChanges()
{
	std::ofstream outputFile(m_FilePath);

	if (outputFile.is_open())	
	{
		outputFile << m_LevelChanges;

		outputFile.close();
	}
	m_ChangesSaved = true;
}
