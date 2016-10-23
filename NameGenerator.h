#pragma once
#include <iostream>
#include <string>
#include <map>
#include <locale> 
#include <fstream>
#include <time.h>    
class nameGenerator
{
public:
	//0 is any, 1 is front, 2 is back, 4 is single, 5 is either
	nameGenerator();
	std::string getName();
	bool addName(std::string, int);
private:
	void loadTable();
	std::string getRandomName();
	std::string getFrontWord(std::string);
	std::string getBackWord(std::string);
	std::string convertBackword(std::string);
	std::map<std::string, int> nameMap;
	std::map<std::string, bool> existingName;
	bool checkRedundancy(std::string name); //Returns if the given string exists within the existing property name map.
};