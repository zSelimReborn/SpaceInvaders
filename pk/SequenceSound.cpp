#include "SequenceSound.h"

#include "Random.h"

SequenceSound::SequenceSound()
	: CurrentIndex(0)
{
	Paths.clear();
}

SequenceSound::SequenceSound(const std::vector<std::string>& InPaths)
	: SequenceSound()
{
	Paths.assign(InPaths.begin(), InPaths.end());
}

void SequenceSound::Add(const std::string& InPath)
{
	Paths.push_back(InPath);
}

std::string SequenceSound::GetPath()
{
	std::string Empty;
	if (Paths.empty())
	{
		return Empty;
	}

	std::string Sound = Paths[CurrentIndex];
	CurrentIndex = (CurrentIndex + 1) % Paths.size();
	return Sound;
}
