#include "TierGenerator.h"

TierGenerator::TierGenerator()
{
	GenerateTiers();
}

std::string TierGenerator::PrintBasic()
{
	std::string row;
	for (int i = 0; i < properties.size(); i++) {
		row = "";
		row += (char)(i + (int)'0');
		row += ": ";
		for (int j = 0; j < properties[i].size(); j++) {
			row += properties[i][j].GetName() + " ";
		}
		row += "  Elements: ";
		row += (char)((int)'0' + properties[i].size());
		row += "\n";
	}
	return row;
}

std::string TierGenerator::PrintMembers()
{
	std::string row;
	for (int i = 0; i<properties.size(); i++) {
		row = "";
		row += (char)(i + (int)'0');
		row += ": ";
		for (int j = 0; j<properties[i].size(); j++) {
			row += properties[i][j].GetName() + "(";
			for (int k = 0; k<properties[i][j].lowerLinkedProperties.size(); k++)
			{
				row += properties[i][j].lowerLinkedProperties[k].GetName();
				if (k != properties[i][j].lowerLinkedProperties.size() - 1)
					row += ", ";
			}
			row += ") ";
		}
		row += "  Elements: ";
		row += (char)((int)'0' + properties[i].size());
		row += "\n";
	}
	return row;
}

std::string TierGenerator::PrintWeights()
{
	std::string row;
	for (int i = 0; i<properties.size(); i++) {
		row = "";
		row += char((int)'0' + i + 1);
		row += ": ";
		for (int j = 0; j<properties[i].size(); j++) {
			row += properties[i][j].GetName() += "(";
			for (int k = 0; k<properties[i][j].lowerLinkedProperties.size(); k++)
			{
				if (properties[i][j].lowerWeights.size() == properties[i][j].lowerLinkedProperties.size())
					//CryLog("Double: %i", properties[i][j].lowerWeights[k]);
				row += properties[i][j].lowerLinkedProperties[k].GetName();
				if (k != properties[i][j].lowerLinkedProperties.size() - 1)
					row += ", ";
			}
			row += ") ";
		}
		row += "  Elements: ";
		row += (char)((int)'0' + properties[i].size());
		row += "\n";
	}
	return row;
}

void TierGenerator::GenerateTiers()
{
	nameGenerator mainGenerator;
	srand(time(NULL)); //Setup random numbers
	int tiers = rand() % 4 + 7; //Number of tiers
	for (int i = 0; i < tiers; i++) {
		if (i == 0)//For the first tier only
		{
			std::vector<Property> tier1;
			Property prop;
			prop.SetName("Light");
			tier1.push_back(prop);
			prop.SetName("Mass");
			tier1.push_back(prop);
			prop.SetName("Heat");
			tier1.push_back(prop);
			prop.SetName("Color");
			tier1.push_back(prop);
			prop.SetName("Size");
			tier1.push_back(prop);
			prop.SetName("Hardness");
			tier1.push_back(prop);
			prop.SetName("Force");
			tier1.push_back(prop);
			properties.push_back(tier1);
		}
		else //For every tier except the first tier
		{
			properties.push_back(std::vector<Property>());
			int propertiesPerTier = rand() % 4 + 4;//How many properties are in this tier
			for (int j = 0; j<propertiesPerTier; j++) {
				Property prop;
				//prop.SetName(names[rand() % (namesSize)]);
				prop.SetName(mainGenerator.getName());
				properties[i].push_back(prop);
			}
		}
	}

	//Setup property tier linkage
	//First pick number of links
	for (int i = 0; i<properties.size(); i++)
	{
		for (int j = 0; j<properties[i].size(); j++)
		{
			if (i != properties.size() - 1)//Last tier will not have lower members
			{
				properties[i][j].SetMembers(rand() % 3 + 2);
			}
		}
	}
	//Next pick random lower tier members to link
	for (int i = 0; i<properties.size(); i++)
	{
		for (int j = 0; j<properties[i].size(); j++)
		{
			std::vector<Property> lowerProperties; // List of properties in lower tier
			for (int k = 0; k<properties[i + 1].size(); k++)
			{
				lowerProperties.push_back(properties[i + 1][k]); // Populate lower tier list
			}

			while ((properties[i][j].GetMembers() > 0) && lowerProperties.size() > 0)
			{
				int randChoice = rand() % ((int)lowerProperties.size());
				properties[i][j].lowerLinkedProperties.push_back(lowerProperties[randChoice]);
				lowerProperties.erase(lowerProperties.begin() + randChoice);
				properties[i][j].SetMembers(properties[i][j].GetMembers() - 1);
			}
		}
	}
	//Setup Weights so that the sum of the the weights of the lower linked properties (properties it is made of) = 1
	for (int x = 0; x<properties.size(); x++)
	{
		for (int y = 0; y<properties[x].size(); y++)
		{
			int numOfMembers = properties[x][y].lowerLinkedProperties.size();
			int sum = 0;

			int* randomNums = new int[numOfMembers];
			double* weights = new double[numOfMembers];

			for (int i = 0; i<numOfMembers; i++)
			{
				randomNums[i] = rand();
			}
			for (int i = 0; i<numOfMembers; i++)
			{
				sum += randomNums[i];
			}
			for (int i = 0; i<numOfMembers; i++)
			{
				properties[x][y].lowerWeights.push_back(randomNums[i] / (double)sum);
				weights[i] = randomNums[i] / (double)sum;
			}
		}
	}
}

std::vector<std::vector<Property>> TierGenerator::GetProperties()
{
	return properties;
}