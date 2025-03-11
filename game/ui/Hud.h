#pragma once

#include "../../pk/ui/Widget.h"

#include <string>

using namespace pk;

class Hud : public Widget
{
public:
	Hud(std::string InFontName);

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