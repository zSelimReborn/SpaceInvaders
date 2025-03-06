#include "MainMenu.h"

#include <iostream>
#include <GLFW/glfw3.h>

#include "Assets.h"
#include "Game.h"
#include "pk/AssetManager.h"
#include "pk/Common.h"
#include "pk/SaveSystem.h"
#include "pk/SoundEngine.h"

MainMenu::MainMenu(const GameWeakPtr& InGame)
	: CurrentChoice(0), MaxChoice(2), GamePtr(InGame)
{
	SetScene(InGame);
}

void MainMenu::Input(const InputHandler& Handler, const float Delta)
{
	Widget::Input(Handler, Delta);

	if (Handler.IsPressed(GLFW_KEY_ENTER) || Handler.IsPadPressed(GLFW_GAMEPAD_BUTTON_CROSS))
	{
		HandleChoice();
	}
	else if (Handler.IsPressed(GLFW_KEY_UP) || Handler.IsPadPressed(GLFW_GAMEPAD_BUTTON_DPAD_UP))
	{
		int OldChoice = CurrentChoice;
		CurrentChoice = std::max(0, CurrentChoice - 1);
		if (OldChoice != CurrentChoice)
		{
			OnChangeChoice();
		}
	}
	else if (Handler.IsPressed(GLFW_KEY_DOWN) || Handler.IsPadPressed(GLFW_GAMEPAD_BUTTON_DPAD_DOWN))
	{
		int OldChoice = CurrentChoice;
		CurrentChoice = std::min(MaxChoice, CurrentChoice + 1);
		if (OldChoice != CurrentChoice)
		{
			OnChangeChoice();
		}
	}
}

void MainMenu::Render()
{
	Widget::Render();

	const Game::SharedPtr CurrentScene = GetGame();
	if (CurrentScene == nullptr)
	{
		return;
	}

	const std::string TitleText = "SPACE INVADERS", StartText = "START GAME", QuitText = "QUIT";
	const glm::vec2 Center(CurrentScene->GetScreenCenter());
	const glm::vec2 TitlePos(Center.x, Center.y - 150.f);
	const glm::vec2 StartPos(Center.x, Center.y - 90.f);
	const glm::vec2 MutePos(Center.x, Center.y - 55.f);
	const glm::vec2 QuitPos(Center.x, Center.y - 20.f);

	glm::vec2 TitleSize, StartSize, MuteSize, QuitSize;
	std::string MuteText = "MUTE: ";
	MuteText += (CurrentScene->IsMuted()) ? "YES" : "NO";

	RenderText(Assets::Fonts::HeadingFontName, TitlePos, TitleText, TextOrient::Center, 1.0f, Colors::White, TitleSize.x, TitleSize.y);
	RenderText(Assets::Fonts::TextFontName, StartPos, StartText, TextOrient::Center, 1.0f, Colors::White, StartSize.x, StartSize.y);
	RenderText(Assets::Fonts::TextFontName, MutePos, MuteText, TextOrient::Center, 1.0f, Colors::White, MuteSize.x, MuteSize.y);
	RenderText(Assets::Fonts::TextFontName, QuitPos, QuitText, TextOrient::Center, 1.0f, Colors::White, QuitSize.x, QuitSize.y);

	std::string SelectedText = StartText;
	glm::vec2 SelectedPos = StartPos;
	glm::vec2 SelectedSize = StartSize;
	if (CurrentChoice == 1)
	{
		SelectedText = MuteText;
		SelectedPos = MutePos;
		SelectedSize = MuteSize;
	}
	else if (CurrentChoice == 2)
	{
		SelectedText = QuitText;
		SelectedPos = QuitPos;
		SelectedSize = QuitSize;
	}

	RenderSelectArrows(SelectedText, SelectedPos, SelectedSize);
}

MainMenu::GameSharedPtr MainMenu::GetGame() const
{
	return GamePtr.lock();
}

void MainMenu::OnChangeChoice() const
{
	PlayNavSound();
}

void MainMenu::HandleChoice()
{
	switch (CurrentChoice)
	{
	case 1:
		ToggleMute();
		break;
	case 2:
		QuitGame();
		break;
	case 0:
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
