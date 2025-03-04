#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <glm/detail/type_vec.hpp>

class ClassSettings
{
public:
	typedef std::shared_ptr<ClassSettings> SharedPtr;
	typedef std::shared_ptr<const ClassSettings> SharedConstPtr;

	typedef std::string Key;
	typedef std::string Value;
	typedef std::vector<int> IntList;
	typedef std::vector<float> FloatList;
	typedef std::vector<Value> StringList;
	typedef glm::vec2 Vec2;
	typedef glm::vec3 Vec3;
	typedef glm::vec4 Vec4;
	typedef std::map<Key, Value> SettingsMap;

	ClassSettings();
	ClassSettings(const SettingsMap& InMap);

	void SetMap(const SettingsMap& InMap);

	bool Exists(const Key& InKey) const;

	bool Get(const Key& InKey, const int Default, int& OutValue) const;
	bool Get(const Key& InKey, const float Default, float& OutValue) const;
	bool Get(const Key& InKey, const std::string& Default, Value& OutValue) const;

	bool Get(const Key& InKey, IntList& OutList) const;
	bool Get(const Key& InKey, FloatList& OutList) const;
	bool Get(const Key& InKey, StringList& OutList) const;

	bool Get(const Key& InKey, Vec2& OutVec) const;
	bool Get(const Key& InKey, Vec3& OutVec) const;
	bool Get(const Key& InKey, Vec4& OutVec) const;

private:
	static StringList Split(const Value& InValue);
	static FloatList AsFloat(const StringList& InList);

	SettingsMap Map;
};
