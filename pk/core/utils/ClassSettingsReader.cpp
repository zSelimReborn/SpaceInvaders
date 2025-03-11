#include "ClassSettingsReader.h"

#include <fstream>
#include <iostream>
#include <sstream>

using namespace pk;

ClassSettingsReader::Map ClassSettingsReader::AllSettings{};

ClassSettings::SharedConstPtr ClassSettingsReader::Load(const Map::key_type& InPath)
{
	if (Exists(InPath))
	{
		return Get(InPath);
	}

	ClassSettings::SharedConstPtr NewSettings = ReadFile(InPath);
    if (NewSettings == nullptr)
    {
        return nullptr;
    }

	AllSettings.insert(MapPair(InPath, NewSettings));
	return NewSettings;
}

ClassSettings::SharedConstPtr ClassSettingsReader::Get(const Map::key_type& InPath)
{
    if (!Exists(InPath))
    {
        return nullptr;
    }

	return AllSettings[InPath];
}

bool ClassSettingsReader::Exists(const Map::key_type& InPath)
{
    return AllSettings.count(InPath) > 0;
}

ClassSettings::SharedConstPtr ClassSettingsReader::ReadFile(const std::string& InPath)
{
	ClassSettings::SharedPtr NewSettings = std::make_shared<ClassSettings>();
    ClassSettings::SettingsMap NewSettingsMap;

    std::ifstream Handler;
    Handler.exceptions(std::ifstream::badbit);

    try {
        Handler.open(InPath);
        std::string Line;
        while (std::getline(Handler, Line))
        {
            if (Line.empty())
            {
                continue;
            }

            SettingPair NewPair = Split(Line);
            NewSettingsMap.insert(NewPair);
        }
    }
    catch (const std::ifstream::failure& e) {
        std::cout << "[ClassSettingsReader] - Error reading setting file: " << InPath << " " << e.what() << "\n";
        return nullptr;
    }

    NewSettings->SetMap(NewSettingsMap);
	return NewSettings;
}

ClassSettingsReader::SettingPair ClassSettingsReader::Split(const std::string& Line)
{
    SettingPair Pair;
    std::stringstream Stream(Line);

    std::getline(Stream, Pair.first, '=');
    std::getline(Stream, Pair.second, '=');

    return Pair;
}

