#pragma once
#include "BodyModel.h"
#include <math_functions.h>
#include <string>
#include "BodyModelBlueprint.h"

class BodyModelSerializer
{
private:
	std::string m_FilePath;
	std::string m_SerializedBodyModels;

public:
	void setLevelFilepath(const std::string& filepath)
	{
		m_FilePath = filepath;
	}

	void discardChanges() 
	{
		m_SerializedBodyModels = "";
	}

	void serializeModel(BodyModelBlueprint model);
	void saveChanges();
};

