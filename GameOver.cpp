#include "GameOver.h"

#include <iostream>
#include <GLFW/glfw3.h>

#include "Assets.h"
#include "Game.h"
#include "pk/Common.h"
#include "pk/InputHandler.h"

GameOver::GameOver(const GameWeakPtr& InGame)
	: GamePtr(InGame)
{
}

void GameOver::Input(const InputHandler& Handler, const float Delta)
{
	Widget::Input(Handler, Delta);

	if (Handler.IsPressed(GLFW_KEY_ENTER))
	{
		HandleInput();
	}
}

void GameOver::Render()
{
	Widget::Render();

	const Scene::SharedPtr CurrentScene = GetScene();
	if (CurrentScene == nullptr)
	{
		return;
	}

	const glm::vec2 Center(CurrentScene->GetScreenCenter());
	const glm::vec2 TitlePos(Center.x, Center.y - 150.f);
	const glm::vec2 RestartPos(Center.x, Center.y - 90.f);

	glm::vec2 TitleSize, RestartSize;
	RenderText(Assets::Fonts::HeadingFontName, TitlePos, "GAME OVER!", TextOrient::Center, 1.0f, Colors::White, TitleSize.x, TitleSize.y);
	RenderText(Assets::Fonts::TextFontName, RestartPos, "Restart", TextOrient::Center, 1.0f, Colors::White, RestartSize.x, RestartSize.y);
	RenderSelectArrows(RestartPos, RestartSize);
}

void GameOver::HandleInput() const
{
	const GameSharedPtr	CurrentGame = GetGame();
	if (CurrentGame == nullptr)
	{
		std::cout << "Unable to restart, empty game object\n";
		return;
	}

	CurrentGame->Play();
}

void GameOver::RenderSelectArrows(const glm::vec2& OptionPos, const glm::vec2& OptionSize) const
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

GameOver::GameSharedPtr GameOver::GetGame() const
{
	return GamePtr.lock();
}
