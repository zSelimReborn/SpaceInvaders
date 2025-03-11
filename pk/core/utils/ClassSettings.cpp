#include "ClassSettings.h"

#include <sstream>
#include <glm/gtc/matrix_transform.hpp>

using namespace pk;

ClassSettings::ClassSettings() = default;

ClassSettings::ClassSettings(const SettingsMap& InMap)
	: Map(InMap)
{
}

void ClassSettings::SetMap(const SettingsMap& InMap)
{
	Map = InMap;
}

bool ClassSettings::Exists(const Key& InKey) const
{
	return Map.count(InKey) > 0;
}

bool ClassSettings::Get(const Key& InKey, const int Default, int& OutValue) const
{
	if (!Exists(InKey))
	{
		OutValue = Default;
		return false;
	}

	const Value TempValue = Map.at(InKey);
	OutValue = std::stoi(TempValue);
	return true;
}

bool ClassSettings::Get(const Key& InKey, const float Default, float& OutValue) const
{
	if (!Exists(InKey))
	{
		OutValue = Default;
		return false;
	}

	const Value TempValue = Map.at(InKey);
	OutValue = std::stof(TempValue);
	return true;
}

bool ClassSettings::Get(const Key& InKey, const std::string& Default, Value& OutValue) const
{
	if (!Exists(InKey))
	{
		OutValue = Default;
		return false;
	}

	OutValue = Map.at(InKey);
	return true;
}

bool ClassSettings::Get(const Key& InKey, IntList& OutList) const
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

bool ClassSettings::Get(const Key& InKey, FloatList& OutList) const
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

bool ClassSettings::Get(const Key& InKey, StringList& OutList) const
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

bool ClassSettings::Get(const Key& InKey, Vec2& OutVec) const
{
	if (!Exists(InKey))
	{
		return false;
	}

	const Value TempValue = Map.at(InKey);
	const StringList Splitted = Split(TempValue);
	if (Splitted.size() < 2)
	{
		return false;
	}

	const FloatList Floats = AsFloat(Splitted);
	OutVec.x = Floats[0];
	OutVec.y = Floats[1];

	return true;
}

bool ClassSettings::Get(const Key& InKey, Vec3& OutVec) const
{
	if (!Exists(InKey))
	{
		return false;
	}

	const Value TempValue = Map.at(InKey);
	const StringList Splitted = Split(TempValue);
	if (Splitted.size() < 3)
	{
		return false;
	}

	const FloatList Floats = AsFloat(Splitted);
	OutVec.x = Floats[0];
	OutVec.y = Floats[1];
	OutVec.z = Floats[2];

	return true;
}

bool ClassSettings::Get(const Key& InKey, Vec4& OutVec) const
{
	if (!Exists(InKey))
	{
		return false;
	}

	const Value TempValue = Map.at(InKey);
	const StringList Splitted = Split(TempValue);
	if (Splitted.size() < 4)
	{
		return false;
	}

	const FloatList Floats = AsFloat(Splitted);
	OutVec.x = Floats[0];
	OutVec.y = Floats[1];
	OutVec.z = Floats[2];
	OutVec.w = Floats[3];

	return true;
}

ClassSettings::StringList ClassSettings::Split(const Value& InValue)
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

ClassSettings::FloatList ClassSettings::AsFloat(const StringList& InList)
{
	FloatList OutList;
	for (const std::string& Element : InList)
	{
		OutList.push_back(std::stof(Element));
	}

	return OutList;
}
