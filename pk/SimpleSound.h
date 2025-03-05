#pragma once
#include "ISound.h"

class SimpleSound : public ISound
{
public:
	SimpleSound(std::string InName);

	std::string GetPath() override;

private:
	std::string Name;
};