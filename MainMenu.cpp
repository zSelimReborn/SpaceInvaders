#include "MainMenu.h"

#include <iostream>
#include <GLFW/glfw3.h>

#include "Assets.h"
#include "Game.h"
#include "pk/Common.h"
#include "pk/SoundEngine.h"

MainMenu::MainMenu(const GameWeakPtr& InGame)
	: CurrentChoice(0), MaxChoice(1), GamePtr(InGame)
{
	SetScene(InGame);
}

void MainMenu::Input(const InputHandler& Handler, const float Delta)
{
	Widget::Input(Handler, Delta);

	if (Handler.IsPressed(GLFW_KEY_ENTER))
	{
		HandleChoice();
	}
	else if (Handler.IsPressed(GLFW_KEY_UP))
	{
		int OldChoice = CurrentChoice;
		CurrentChoice = std::max(0, CurrentChoice - 1);
		if (OldChoice != CurrentChoice)
		{
			OnChangeChoice();
		}
	}
	else if (Handler.IsPressed(GLFW_KEY_DOWN))
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

	const Scene::SharedPtr CurrentScene = GetScene();
	if (CurrentScene == nullptr)
	{
		return;
	}

	const glm::vec2 Center(CurrentScene->GetScreenCenter());
	const glm::vec2 TitlePos(Center.x, Center.y - 150.f);
	const glm::vec2 StartPos(Center.x, Center.y - 90.f);
	const glm::vec2 QuitPos(Center.x, Center.y - 55.f);

	glm::vec2 TitleSize, StartSize, QuitSize;
	RenderText(Assets::Fonts::HeadingFontName, TitlePos, "Space Invaders", TextOrient::Center, 1.0f, Colors::White, TitleSize.x, TitleSize.y);
	RenderText(Assets::Fonts::TextFontName, StartPos, "START GAME", TextOrient::Center, 1.0f, Colors::White, StartSize.x, StartSize.y);
	RenderText(Assets::Fonts::TextFontName, QuitPos, "QUIT", TextOrient::Center, 1.0f, Colors::White, QuitSize.x, QuitSize.y);

	glm::vec2 SelectedPos = StartPos;
	glm::vec2 SelectedSize = StartSize;
	if (CurrentChoice == 1)
	{
		SelectedPos = QuitPos;
		SelectedSize = QuitSize;
	}
	RenderSelectArrows(SelectedPos, SelectedSize);
}

MainMenu::GameSharedPtr MainMenu::GetGame() const
{
	return GamePtr.lock();
}

void MainMenu::OnChangeChoice()
{
	SoundEngine::Get().Play(Assets::Sounds::MenuNavigation, 1.f);
}

void MainMenu::HandleChoice()
{
	if (CurrentChoice == 0)
	{
		StartGame();
	}
	else
	{
		QuitGame();
	}
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

void MainMenu::RenderSelectArrows(const glm::vec2& OptionPos, const glm::vec2& OptionSize) const
{
	const float LeftOffset = (OptionSize.x / 2) + 20.f;
	const float RightOffset = (OptionSize.x / 2) + 10.f;

	const glm::vec2 LeftArrow(OptionPos.x - LeftOffset, OptionPos.y);
	const glm::vec2 RightArrow(OptionPos.x + RightOffset, OptionPos.y);

	const glm::vec4 Color(0.8f, 0.84f, 0.86f, 1.f);

	float OutWidth, OutHeight;
	RenderText(Assets::Fonts::TextFontName, LeftArrow, ">", TextOrient::Left, 1.0f, Color, OutWidth, OutHeight);
	RenderText(Assets::Fonts::TextFontName, RightArrow, "<", TextOrient::Left, 1.0f, Color, OutWidth, OutHeight);
}
