#pragma once

#include <fstream>
#include <vector>

#include "pk/core/save/ISaveFile.h"

struct SaveImpl
{
	static const int MAX_SCORES;

	int bMuted;
	std::vector<int> Scores;

	SaveImpl() : bMuted(false) {}
};

class GameSave : public ISaveFile
{
public:
	GameSave();

	void Load(std::ifstream& Reader) override;
	void Write(std::ofstream& Writer) override;

	bool IsMuted() const;
	void ToggleMute();

	std::vector<int> GetHighScores() const;
	void AddScore(int InScore);
	int GetMaxScores() const;

private:
	SaveImpl CurrentSave;
};