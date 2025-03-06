#include "MainMenu.h"

#include <iostream>
#include <GLFW/glfw3.h>

#include "Assets.h"
#include "Game.h"
#include "pk/AssetManager.h"
#include "pk/Common.h"
#include "pk/SaveSystem.h"
#include "pk/SoundEngine.h"

const int MainMenu::START_GAME_OPTION = 0;
const int MainMenu::SCORES_OPTION = 1;
const int MainMenu::MUTE_OPTION = 2;
const int MainMenu::QUIT_OPTION = 3;
const int MainMenu::MAX_OPTION = 3;

MainMenu::MainMenu(const GameWeakPtr& InGame)
	: CurrentChoice(START_GAME_OPTION), MaxChoice(MAX_OPTION), GamePtr(InGame), CurrentPanel(Panel::Main)
{
	SetScene(InGame);
}

void MainMenu::Input(const InputHandler& Handler, const float Delta)
{
	Widget::Input(Handler, Delta);

	if (Handler.IsPressed(GLFW_KEY_ESCAPE) || Handler.IsPadPressed(GLFW_GAMEPAD_BUTTON_START))
	{
		if (CurrentPanel == Panel::Main)
		{
			QuitGame();
		}
		else
		{
			BackMenu();
		}
	}

	if (Handler.IsPressed(GLFW_KEY_ENTER) || Handler.IsPadPressed(GLFW_GAMEPAD_BUTTON_CROSS))
	{
		HandleChoice();
	}
	else if (Handler.IsPressed(GLFW_KEY_UP) || Handler.IsPadPressed(GLFW_GAMEPAD_BUTTON_DPAD_UP))
	{
		if (CurrentPanel == Panel::Main)
		{
			int OldChoice = CurrentChoice;
			CurrentChoice = std::max(0, CurrentChoice - 1);
			if (OldChoice != CurrentChoice)
			{
				OnChangeChoice();
			}
		}
	}
	else if (Handler.IsPressed(GLFW_KEY_DOWN) || Handler.IsPadPressed(GLFW_GAMEPAD_BUTTON_DPAD_DOWN))
	{
		if (CurrentPanel == Panel::Main)
		{
			int OldChoice = CurrentChoice;
			CurrentChoice = std::min(MaxChoice, CurrentChoice + 1);
			if (OldChoice != CurrentChoice)
			{
				OnChangeChoice();
			}
		}
	}
}

void MainMenu::Render()
{
	Widget::Render();

	if (CurrentPanel == Panel::Main)
	{
		RenderMain();
	}
	else
	{
		RenderScores();
	}
}

MainMenu::GameSharedPtr MainMenu::GetGame() const
{
	return GamePtr.lock();
}

void MainMenu::RenderMain() const
{
	const Game::SharedPtr CurrentScene = GetGame();
	if (CurrentScene == nullptr)
	{
		return;
	}

	const std::string TitleText = "SPACE INVADERS", StartText = "START GAME", QuitText = "QUIT", ScoresText = "HIGH SCORES";
	const glm::vec2 Center(CurrentScene->GetScreenCenter());
	const glm::vec2 TitlePos(Center.x, Center.y - 150.f);
	const glm::vec2 StartPos(Center.x, Center.y - 90.f);
	const glm::vec2 ScoresPos(Center.x, Center.y - 55.f);
	const glm::vec2 MutePos(Center.x, Center.y - 20.f);
	const glm::vec2 QuitPos(Center.x, Center.y + 15.f);

	glm::vec2 TitleSize, StartSize, MuteSize, QuitSize, ScoresSize;
	std::string MuteText = "MUTE: ";
	MuteText += (CurrentScene->IsMuted()) ? "YES" : "NO";

	RenderText(Assets::Fonts::HeadingFontName, TitlePos, TitleText, TextOrient::Center, 1.0f, Colors::White, TitleSize.x, TitleSize.y);
	RenderText(Assets::Fonts::TextFontName, StartPos, StartText, TextOrient::Center, 1.0f, Colors::White, StartSize.x, StartSize.y);
	RenderText(Assets::Fonts::TextFontName, ScoresPos, ScoresText, TextOrient::Center, 1.0f, Colors::White, ScoresSize.x, ScoresSize.y);
	RenderText(Assets::Fonts::TextFontName, MutePos, MuteText, TextOrient::Center, 1.0f, Colors::White, MuteSize.x, MuteSize.y);
	RenderText(Assets::Fonts::TextFontName, QuitPos, QuitText, TextOrient::Center, 1.0f, Colors::White, QuitSize.x, QuitSize.y);

	std::string SelectedText = StartText;
	glm::vec2 SelectedPos = StartPos;
	glm::vec2 SelectedSize = StartSize;
	if (CurrentChoice == SCORES_OPTION)
	{
		SelectedText = ScoresText;
		SelectedPos = ScoresPos;
		SelectedSize = ScoresSize;
	}
	else if (CurrentChoice == MUTE_OPTION)
	{
		SelectedText = MuteText;
		SelectedPos = MutePos;
		SelectedSize = MuteSize;
	}
	else if (CurrentChoice == QUIT_OPTION)
	{
		SelectedText = QuitText;
		SelectedPos = QuitPos;
		SelectedSize = QuitSize;
	}

	RenderSelectArrows(SelectedText, SelectedPos, SelectedSize);
}

void MainMenu::RenderScores() const
{
	const Game::SharedPtr CurrentScene = GetGame();
	if (CurrentScene == nullptr)
	{
		return;
	}

	const std::vector<int> Scores = CurrentScene->GetHighScores();
	const std::string TitleText = "HIGH SCORES", NoDataText = "N/D", BackText = "BACK";
	const glm::vec2 Center(CurrentScene->GetScreenCenter());
	const glm::vec2 TitlePos(Center.x, Center.y - 150.f);
	const glm::vec2 NoDataPos(Center.x, Center.y - 90.f);

	glm::vec2 TitleSize, NoDataSize, CurrentScorePos = NoDataPos;
	RenderText(Assets::Fonts::HeadingFontName, TitlePos, TitleText, TextOrient::Center, 1.0f, Colors::White, TitleSize.x, TitleSize.y);
	if (!Scores.empty())
	{
		
		for (int i = 0; i < Scores.size() && i < CurrentScene->GetMaxScores(); ++i)
		{
			const int Score = Scores[i];
			const int Position = i + 1;
			std::string ScoreText = std::to_string(Position);
			ScoreText += ". ";
			ScoreText += std::to_string(Score);

			glm::vec2 ScoreSize;
			RenderText(Assets::Fonts::TextFontName, CurrentScorePos, ScoreText, TextOrient::Center, 1.0f, Colors::White, ScoreSize.x, ScoreSize.y);

			CurrentScorePos.y += 35.f;
		}
	}
	else
	{
		RenderText(Assets::Fonts::TextFontName, NoDataPos, NoDataText, TextOrient::Center, 1.0f, Colors::White, NoDataSize.x, NoDataSize.y);
		CurrentScorePos.y += 35.f;
	}

	CurrentScorePos.y += 35.f;
	glm::vec2 BackSize;
	RenderText(Assets::Fonts::TextFontName, CurrentScorePos, BackText, TextOrient::Center, 1.0f, Colors::White, BackSize.x, BackSize.y);
	RenderSelectArrows(BackText, CurrentScorePos, BackSize);
}

void MainMenu::OnChangeChoice() const
{
	PlayNavSound();
}

void MainMenu::HandleChoice()
{
	if (CurrentPanel == Panel::Scores)
	{
		BackMenu();
		PlayNavSound();
		return;
	}

	switch (CurrentChoice)
	{
	case SCORES_OPTION:
		ShowScores();
		break;
	case MUTE_OPTION:
		ToggleMute();
		break;
	case QUIT_OPTION:
		QuitGame();
		break;
	case START_GAME_OPTION:
	default:
		StartGame();
	}

	PlayNavSound();
}

void MainMenu::StartGame()
{
	const GameSharedPtr Game = GetGame();
	if (Game == nullptr)
	{
		std::cout << "Unable to start game, empty object\n";
		return;
	}

	Game->Play();
	Deactivate();
}

void MainMenu::ShowScores()
{
	CurrentPanel = Panel::Scores;
}

void MainMenu::ToggleMute() const
{
	const GameSharedPtr Game = GetGame();
	if (Game == nullptr)
	{
		return;
	}

	Game->ToggleMute();
}

void MainMenu::QuitGame()
{
	const GameSharedPtr Game = GetGame();
	if (Game == nullptr)
	{
		std::cout << "Unable to quit game, empty object\n";
		return;
	}

	Game->Quit();
	Deactivate();
}

void MainMenu::BackMenu()
{
	CurrentPanel = Panel::Main;
}

void MainMenu::PlayNavSound() const
{
	const GameSharedPtr CurrentGame = GetGame();
	if (CurrentGame == nullptr)
	{
		SoundEngine::Get().Play(Assets::Sounds::MenuNavigation, 1.f);
	}
	else
	{
		CurrentGame->PlayAudio(Assets::Sounds::MenuNavigationName, 1.f);
	}
}

void MainMenu::RenderSelectArrows(const std::string& InText, const glm::vec2& OptionPos, const glm::vec2& OptionSize) const
{
	Font::SharedPtr Font = AssetManager::Get().GetFont(Assets::Fonts::TextFontName);
	float FirstAdvance = 2.f;
	if (Font != nullptr)
	{
		FirstAdvance += Font->GetCharacterAdvance(InText[0], 1.f);
	}

	const float LeftOffset = (OptionSize.x / 2.f) + FirstAdvance;
	const float RightOffset = (OptionSize.x / 2.f);

	const glm::vec2 LeftArrow(OptionPos.x - LeftOffset, OptionPos.y);
	const glm::vec2 RightArrow(OptionPos.x + RightOffset, OptionPos.y);

	const glm::vec4 Color(0.8f, 0.84f, 0.86f, 1.f);

	float OutWidth, OutHeight;
	RenderText(Assets::Fonts::TextFontName, LeftArrow, ">", TextOrient::Left, 1.0f, Color, OutWidth, OutHeight);
	RenderText(Assets::Fonts::TextFontName, RightArrow, "<", TextOrient::Left, 1.0f, Color, OutWidth, OutHeight);
}
