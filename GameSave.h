#pragma once

#include <fstream>

#include "pk/ISaveFile.h"

class GameSave : public ISaveFile
{
public:
	GameSave();

	void Load(std::ifstream& Reader) override;
	void Write(std::ofstream& Writer) override;

	bool IsMuted() const;
	void ToggleMute();

private:
	int bMuted;
};