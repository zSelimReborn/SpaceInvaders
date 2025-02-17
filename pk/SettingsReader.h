#pragma once

#include "Settings.h"
#include <map>

class SettingsReader
{
public:
	typedef std::map<std::string, Settings::SharedConstPtr> Map;
	typedef std::pair<Map::key_type, Settings::SharedConstPtr> MapPair;
	typedef std::pair<Settings::Key, Settings::Value> SettingPair;

	static Settings::SharedConstPtr Load(const Map::key_type& InPath);
	static Settings::SharedConstPtr Get(const Map::key_type& InPath);
	static bool Exists(const Map::key_type& InPath);
private:
	static Settings::SharedConstPtr ReadFile(const std::string& InPath);
	static SettingPair Split(const std::string& Line);

	static Map AllSettings;
};