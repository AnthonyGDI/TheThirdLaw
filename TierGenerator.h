#pragma once
#include "Property.h"
#include "NameGenerator.h"

class TierGenerator
{
	std::vector<std::vector<Property>> properties;
public:
	TierGenerator();
	std::string PrintBasic();
	std::string PrintMembers();
	std::string PrintWeights();
	void GenerateTiers();
	std::vector<std::vector<Property>> GetProperties();
};
