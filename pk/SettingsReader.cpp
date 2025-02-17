#include "SettingsReader.h"

#include <fstream>
#include <iostream>
#include <sstream>

SettingsReader::Map SettingsReader::AllSettings{};

Settings::SharedConstPtr SettingsReader::Load(const Map::key_type& InPath)
{
	if (Exists(InPath))
	{
		return Get(InPath);
	}

	Settings::SharedConstPtr NewSettings = ReadFile(InPath);
    if (NewSettings == nullptr)
    {
        return nullptr;
    }

	AllSettings.insert(MapPair(InPath, NewSettings));
	return NewSettings;
}

Settings::SharedConstPtr SettingsReader::Get(const Map::key_type& InPath)
{
    if (!Exists(InPath))
    {
        return nullptr;
    }

	return AllSettings[InPath];
}

bool SettingsReader::Exists(const Map::key_type& InPath)
{
    return AllSettings.count(InPath) > 0;
}

Settings::SharedConstPtr SettingsReader::ReadFile(const std::string& InPath)
{
	Settings::SharedPtr NewSettings = std::make_shared<Settings>();
    Settings::SettingsMap NewSettingsMap;

    std::ifstream Handler;
    Handler.exceptions(std::ifstream::badbit | std::ifstream::failbit);

    try {
        Handler.open(InPath);
        while (!Handler.eof())
        {
            std::string Line;
            std::getline(Handler, Line);
            SettingPair NewPair = Split(Line);
            NewSettingsMap.insert(NewPair);
        }

        Handler.close();
    }
    catch (const std::ifstream::failure& e) {
        std::cout << "[SettingsReader] - Error reading setting file: " << InPath << " " << e.what() << "\n";
        return nullptr;
    }

    NewSettings->SetMap(NewSettingsMap);
	return NewSettings;
}

SettingsReader::SettingPair SettingsReader::Split(const std::string& Line)
{
    SettingPair Pair;
    std::stringstream Stream(Line);

    std::getline(Stream, Pair.first, '=');
    std::getline(Stream, Pair.second, '=');

    return Pair;
}

