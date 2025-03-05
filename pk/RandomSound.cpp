#include "RandomSound.h"

#include "Random.h"

RandomSound::RandomSound()
{
	Paths.clear();
}

RandomSound::RandomSound(const std::vector<std::string>& InPaths)
	: RandomSound()
{
	Paths.assign(InPaths.begin(), InPaths.end());
}

void RandomSound::Add(const std::string& InPath)
{
	for (const std::string& Path : Paths)
	{
		if (Path == InPath)
		{
			return;
		}
	}

	Paths.push_back(InPath);
}

std::string RandomSound::GetPath()
{
	std::string Empty;
	if (Paths.empty())
	{
		return Empty;
	}

	int NameCount = static_cast<int>(Paths.size());
	int RandomIndex = Random::Get(0, NameCount - 1);
	return Paths[RandomIndex];
}
