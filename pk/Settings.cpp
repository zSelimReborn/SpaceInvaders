#include "Settings.h"

#include <sstream>

Settings::Settings()
	: Map()
{
}

Settings::Settings(const SettingsMap& InMap)
	: Map(InMap)
{
}

void Settings::SetMap(const SettingsMap& InMap)
{
	Map = InMap;
}

bool Settings::Exists(const Key& InKey) const
{
	return Map.count(InKey) > 0;
}

bool Settings::Get(const Key& InKey, int& OutValue) const
{
	if (!Exists(InKey))
	{
		return false;
	}

	const Value TempValue = Map.at(InKey);
	OutValue = std::stoi(TempValue);
	return true;
}

bool Settings::Get(const Key& InKey, float& OutValue) const
{
	if (!Exists(InKey))
	{
		return false;
	}

	const Value TempValue = Map.at(InKey);
	OutValue = std::stof(TempValue);
	return true;
}

bool Settings::Get(const Key& InKey, Value& OutValue) const
{
	if (!Exists(InKey))
	{
		return false;
	}

	OutValue = Map.at(InKey);
	return true;
}

bool Settings::GetList(const Key& InKey, IntList& OutList) const
{
	if (!Exists(InKey))
	{
		return false;
	}

	const Value TempValue = Map.at(InKey);
	StringList TempList = Split(TempValue);

	OutList.clear();
	for (const Value& Element : TempList)
	{
		OutList.push_back(std::stoi(Element));
	}

	return true;
}

bool Settings::GetList(const Key& InKey, FloatList& OutList) const
{
	if (!Exists(InKey))
	{
		return false;
	}

	const Value TempValue = Map.at(InKey);
	StringList TempList = Split(TempValue);

	OutList.clear();
	for (const Value& Element : TempList)
	{
		OutList.push_back(std::stof(Element));
	}

	return true;
}

bool Settings::GetList(const Key& InKey, StringList& OutList) const
{
	if (!Exists(InKey))
	{
		return false;
	}

	const Value TempValue = Map.at(InKey);
	OutList.clear();
	OutList = Split(TempValue);
	return true;
}

Settings::StringList Settings::Split(const Value& InValue)
{
	std::stringstream Stream(InValue);
	StringList Result;

	while (Stream.good())
	{
		Value Element;
		std::getline(Stream, Element, ',');
		Result.push_back(Element);
	}

	return Result;
}
