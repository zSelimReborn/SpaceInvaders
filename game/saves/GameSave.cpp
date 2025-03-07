#include "GameSave.h"

#include <algorithm>

#include "../../pk/core/utils/Common.h"

const int SaveImpl::MAX_SCORES = 5;

GameSave::GameSave() = default;

void GameSave::Load(std::ifstream& Reader)
{
	if (!Reader.good())
	{
		return;
	}

	int TotalScores = 0;
	Reader.seekg(0, std::ios::beg);
	Reader.read(reinterpret_cast<char*>(&CurrentSave.bMuted), sizeof(CurrentSave.bMuted));
	Reader.read(reinterpret_cast<char*>(&TotalScores), sizeof(TotalScores));

	CurrentSave.Scores.clear();
	for (int i = 0; i < TotalScores; ++i)
	{
		if (!Reader.good()) { break; }

		int Score = 0;
		Reader.read(reinterpret_cast<char*>(&Score), sizeof(Score));
		CurrentSave.Scores.push_back(Score);
	}
}

void GameSave::Write(std::ofstream& Writer)
{
	if (!Writer.good())
	{
		return;
	}

	Writer.seekp(0, std::ios::beg);
	Writer.write(reinterpret_cast<char*>(&CurrentSave.bMuted), sizeof(CurrentSave.bMuted));
	int TotalScores = Math::Clamp(CurrentSave.Scores.size(), 0, SaveImpl::MAX_SCORES);
	Writer.write(reinterpret_cast<char*>(&TotalScores), sizeof(TotalScores));

	if (CurrentSave.Scores.empty()) { return; }
	for (int i = 0; i < SaveImpl::MAX_SCORES && i < CurrentSave.Scores.size(); ++i)
	{
		int Score = CurrentSave.Scores[i];
		Writer.write(reinterpret_cast<char*>(&Score), sizeof(Score));
	}
}

bool GameSave::IsMuted() const
{
	return CurrentSave.bMuted;
}

void GameSave::ToggleMute()
{
	CurrentSave.bMuted = !CurrentSave.bMuted;
}

std::vector<int> GameSave::GetHighScores() const
{
	return CurrentSave.Scores;
}

void GameSave::AddScore(int InScore)
{
	if (InScore <= 0)
	{
		return;
	}

	CurrentSave.Scores.push_back(InScore);
	auto Pos = CurrentSave.Scores.end() - 1;
	for (auto It = CurrentSave.Scores.begin(), End = CurrentSave.Scores.end(); It != End; ++It)
	{
		if (InScore > *It)
		{
			Pos = It;
			break;
		}
	}

	if (Pos != CurrentSave.Scores.end() -1)
	{
		for (auto It = CurrentSave.Scores.end() - 1; It != Pos; --It)
		{
			std::iter_swap(It - 1, It);
		}

		*Pos = InScore;
	}
}

int GameSave::GetMaxScores() const
{
	return CurrentSave.MAX_SCORES;
}
