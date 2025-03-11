#include "SimpleSound.h"

using namespace pk;

SimpleSound::SimpleSound(std::string InName)
	: Name(std::move(InName))
{
}

std::string SimpleSound::GetPath()
{
	return Name;
}
