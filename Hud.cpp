#include "Hud.h"

#include <iostream>
#include <sstream>

#include "pk/AssetManager.h"
#include "pk/Common.h"
#include "pk/Scene.h"

Hud::Hud(const std::string& InFontName)
	: LifePoints(0), Score(0), FontName(InFontName)
{
}

void Hud::SetLifePoints(int InLifePoints)
{
	LifePoints = InLifePoints;
}

void Hud::SetScore(int InScore)
{
	Score = InScore;
}

int Hud::GetLifePoints() const
{
	return LifePoints;
}

int Hud::GetScore() const
{
	return Score;
}

void Hud::Render()
{
	Widget::Render();

	const Scene::SharedPtr CurrentScene = GetScene();
	if (CurrentScene == nullptr)
	{
		return;
	}

	const glm::vec2 Center(CurrentScene->GetScreenCenter());

	std::stringstream LifePointsText, ScoreText;
	LifePointsText << "LifePoints: " << LifePoints;
	ScoreText << "Score: " << Score;

	const glm::vec2 LifePointPos = glm::vec2(15.f, 15.f);
	const glm::vec2 ScorePos = glm::vec2(Center.x, 15.f);

	RenderText(FontName, LifePointPos, LifePointsText.str(), TextOrient::Left, 1.0f, Colors::White);
	RenderText(FontName, ScorePos, ScoreText.str(), TextOrient::Center, 1.0f, Colors::White);
}
