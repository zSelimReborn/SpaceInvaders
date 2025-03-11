#pragma once

#include "ClassSettings.h"
#include <map>

namespace pk
{
	class ClassSettingsReader
	{
	public:
		typedef std::map<std::string, ClassSettings::SharedConstPtr> Map;
		typedef std::pair<Map::key_type, ClassSettings::SharedConstPtr> MapPair;
		typedef std::pair<ClassSettings::Key, ClassSettings::Value> SettingPair;

		static ClassSettings::SharedConstPtr Load(const Map::key_type& InPath);
		static ClassSettings::SharedConstPtr Get(const Map::key_type& InPath);
		static bool Exists(const Map::key_type& InPath);
	private:
		static ClassSettings::SharedConstPtr ReadFile(const std::string& InPath);
		static SettingPair Split(const std::string& Line);

		static Map AllSettings;
	};
}