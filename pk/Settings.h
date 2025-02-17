#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

class Settings
{
public:
	typedef std::shared_ptr<Settings> SharedPtr;
	typedef std::shared_ptr<const Settings> SharedConstPtr;

	typedef std::string Key;
	typedef std::string Value;
	typedef std::vector<int> IntList;
	typedef std::vector<float> FloatList;
	typedef std::vector<Value> StringList;
	typedef std::map<Key, Value> SettingsMap;

	Settings();
	Settings(const SettingsMap& InMap);

	void SetMap(const SettingsMap& InMap);

	bool Exists(const Key& InKey) const;

	bool Get(const Key& InKey, int& OutValue) const;
	bool Get(const Key& InKey, float& OutValue) const;
	bool Get(const Key& InKey, Value& OutValue) const;

	bool GetList(const Key& InKey, IntList& OutList) const;
	bool GetList(const Key& InKey, FloatList& OutList) const;
	bool GetList(const Key& InKey, StringList& OutList) const;

private:
	static StringList Split(const Value& InValue);

	SettingsMap Map;
};