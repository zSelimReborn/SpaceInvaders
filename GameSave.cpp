#include "GameSave.h"

GameSave::GameSave()
	: bMuted(0)
{
}

void GameSave::Load(std::ifstream& Reader)
{
	if (!Reader.good())
	{
		return;
	}

	Reader.seekg(0, std::ios::beg);
	Reader.read(reinterpret_cast<char*>(&bMuted), sizeof(bMuted));
}

void GameSave::Write(std::ofstream& Writer)
{
	if (!Writer.good())
	{
		return;
	}

	Writer.seekp(0, std::ios::beg);
	Writer.write(reinterpret_cast<char*>(&bMuted), sizeof(bMuted));
}

bool GameSave::IsMuted() const
{
	return bMuted;
}

void GameSave::ToggleMute()
{
	bMuted = !bMuted;
}
