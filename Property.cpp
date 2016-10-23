#include "Property.h"

Property::Property() 
{
	name = "";
	members = 0;
	links = 0;
}

std::string Property::GetName()
{
	return name;
}

int Property::GetMembers()
{
	return members;
}

int Property::GetLinks()
{
	return links;
}

bool Property::SetName(std::string str)
{
	name = str;
	return true;
}

bool Property::SetMembers(int num)
{
	members = num;
	return true;
}

bool Property::SetLink(int num)
{
	links = num;
	return true;
}