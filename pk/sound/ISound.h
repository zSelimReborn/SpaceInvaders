#pragma once

#include <string>

class ISound
{
public:
	virtual std::string GetPath() = 0;
	virtual ~ISound() = default;
};