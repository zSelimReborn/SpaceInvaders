#pragma once

#include "pk/Widget.h"

#include <string>

class Hud : public Widget
{
public:
	Hud(const std::string& InFontName);

	void SetLifePoints(int InLifePoints);
	void SetScore(int InScore);

	int GetLifePoints() const;
	int GetScore() const;

	void Render() override;

private:
	int LifePoints;
	int Score;

	std::string FontName;
};