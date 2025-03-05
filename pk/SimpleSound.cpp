#include "SimpleSound.h"

SimpleSound::SimpleSound(std::string InName)
	: Name(std::move(InName))
{
}

std::string SimpleSound::GetPath()
{
	return Name;
}
