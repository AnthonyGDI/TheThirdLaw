#pragma once
#include <vector>

class Property
{
	std::string name;
	int members;
	int links;
public:
	Property();
	std::vector <Property> lowerLinkedProperties;
	std::vector <double> lowerWeights;
	std::vector <Property> upperLinkedProperties;
	std::vector <double> upperWeights;
	std::string GetName();
	int GetMembers();
	int GetLinks();
	bool SetName(std::string str);
	bool SetMembers(int num);
	bool SetLink(int num);
};