#pragma once
#include <string>
class ObjectSerializationTags
{
public:
	static const std::string START_OBJECT;
	static const std::string END_OBJECT;
	
	static const std::string START_TYPE;
	static const std::string END_TYPE;
	
	static const std::string START_POSITION;
	static const std::string END_POSITION;
	
	static const std::string START_SIZE;
	static const std::string END_SIZE;

	static const std::string START_ROTATION;
	static const std::string END_ROTATION;
	
	static const std::string START_MAX_HEALTH;
	static const std::string END_MAX_HEALTH;

	static const std::string START_COLOR;
	static const std::string END_COLOR;

	static const std::string START_MATERIAL_DAMAGE_MULTIPLIER;
	static const std::string END_MATERIAL_DAMAGE_MULTIPLIER;
};

