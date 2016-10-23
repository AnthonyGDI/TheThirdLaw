#include "NameGenerator.h"
using namespace std;
nameGenerator::nameGenerator()
{
	srand(time(NULL));
	loadTable();
}

bool nameGenerator::checkRedundancy(std::string nametoCheck)
{
	if (existingName.count(nametoCheck) == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}
std::string nameGenerator::getName()
{

	std::string frontWord;
	std::string backWord;
	std::string currentWord = getRandomName();
	switch (nameMap[currentWord])
	{
	case 0: 
		//Determine if whether or not to return just the word
		if (rand() == 0)
		{
			if (!checkRedundancy(currentWord))
			{
				existingName.insert(std::pair<std::string, bool>(currentWord, true));
				return currentWord;
			}
			else
			{
				return getName();
			}
		}
		else
		{
			//Determine if word is front or back
			if (rand() == 0)
			{
				frontWord = currentWord;
				backWord = getBackWord(frontWord);
				backWord = convertBackword(backWord);
				if (!checkRedundancy(frontWord + backWord))
				{
					existingName.insert(std::pair<std::string, bool>(frontWord + backWord, true));
					return frontWord + backWord;
				}
				else
				{
					return getName();
				}

			}
			else
			{
				backWord = currentWord;
				frontWord = getFrontWord(backWord);
				backWord = convertBackword(backWord);
				if (!checkRedundancy(frontWord + backWord))
				{
					existingName.insert(std::pair<std::string, bool>(frontWord + backWord, true));
					return frontWord + backWord;
				}
				else
				{
					return getName();
				}
			}
		}


	case 1:
		frontWord = currentWord;
		backWord = getBackWord(frontWord);
		backWord = convertBackword(backWord);
		if (!checkRedundancy(frontWord + backWord))
		{
			existingName.insert(std::pair<std::string, bool>(frontWord + backWord, true));
			return frontWord + backWord;
		}
		else
		{
			return getName();
		}

	case 2:
		backWord = currentWord;
		frontWord = getFrontWord(backWord);
		backWord = convertBackword(backWord);
		if (!checkRedundancy(frontWord + backWord))
		{
			existingName.insert(std::pair<std::string, bool>(frontWord + backWord, true));
			return frontWord + backWord;
		}
		else
		{
			return getName();
		}

	case 4:
		if (!checkRedundancy(currentWord))
		{
			existingName.insert(std::pair<std::string, bool>(currentWord, true));
			return currentWord;
		}
		else
		{
			return getName();
		}

	case 5:
		//Determine if word is front or back
		if (rand() == 0)
		{
			frontWord = currentWord;
			backWord = getBackWord(frontWord);
			backWord = convertBackword(backWord);
			if (!checkRedundancy(frontWord + backWord))
			{
				existingName.insert(std::pair<std::string, bool>(frontWord + backWord, true));
				return frontWord + backWord;
			}
			else
			{
				return getName();
			}
		}
		else
		{
			backWord = currentWord;
			frontWord = getFrontWord(backWord);
			backWord = convertBackword(backWord);
			if (!checkRedundancy(frontWord + backWord))
			{
				existingName.insert(std::pair<std::string, bool>(frontWord + backWord, true));
				return frontWord + backWord;
			}
			else
			{
				return getName();
			}
		}

	default: break;
	}
	return "NULL";

}

std::string nameGenerator::convertBackword(std::string backWord)
{
	std::locale loc;
	std::string newString = backWord;
	newString[0] = std::tolower(newString[0], loc);
	return newString;
}

std::string nameGenerator::getRandomName()
{
	auto it = nameMap.begin();
	std::advance(it, rand() % nameMap.size());
	std::string random_key = it->first;
	return random_key;
}

std::string nameGenerator::getFrontWord(std::string backWord)
{
	std::string currentWord = "";
	do
	{
		currentWord = getRandomName();
	} while (currentWord == backWord || (nameMap[currentWord] != 0 && nameMap[currentWord] != 1 && nameMap[currentWord] != 5));
	return currentWord;
}

std::string nameGenerator::getBackWord(std::string frontWord)
{
	std::string currentWord = "";
	do
	{
		currentWord = getRandomName();
	} while (currentWord != frontWord || (nameMap[currentWord] != 0 && nameMap[currentWord] != 2 && nameMap[currentWord] != 5));
	return currentWord;
}
bool nameGenerator::addName(std::string name, int type)
{
	nameMap.insert(std::pair<std::string, int>(name, type));
	return true;
}
void nameGenerator::loadTable()
{
	ifstream if_loadFile("names_property.ini");
	string processorString;
	size_t processorPos;

	string tempName;
	int tempType;
	if (if_loadFile.is_open())
	{
		while (std::getline(if_loadFile, processorString))
		{
			//CryLog("I FOUND A WORD OMGOMGOMG");
			//Find the first instance of a colon, which splits each line with an entry.
			processorPos = processorString.find(':');

			//Split that intro two strings: A number and a name, and add them to the table.
			tempName = processorString.substr(0, processorPos);
			tempType = stoi(processorString.substr(processorPos + 1));

			addName(tempName, tempType);
		}
	}
	else
	{
		addName("error", 5);
		addName("error", 5);
		addName("error", 5);
		addName("error", 5);
		addName("error", 5);
		addName("error", 5);
		addName("error", 5);
		addName("error", 5);
		addName("error", 5);
		addName("error", 5);
		addName("error", 5);
		addName("error", 5);
		addName("error", 5);
		addName("error", 5);
		addName("error", 5);
		addName("error", 5);
		addName("error", 5);
		addName("error", 5);
		addName("error", 5);
		addName("t", 5);
		addName("u", 5);
		addName("v", 5);
		addName("w", 5);
		addName("x", 5);
		addName("y", 5);
		addName("z", 5);
		addName("aa", 5);
		addName("bb", 5);
		addName("cc", 5);
		addName("dd", 5);
		addName("ee", 5);
		addName("ff", 5);
		addName("gg", 5);
		addName("hh", 5);
		addName("ii", 5);
		addName("jj", 5);
		addName("kk", 5);
		addName("ll", 5);
		addName("mm", 5);
		addName("nn", 5);
	}
	if_loadFile.close();

}