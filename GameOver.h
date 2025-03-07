#pragma once

#include "pk/ui/Widget.h"

class Game;

class GameOver : public Widget
{
public:
	typedef std::shared_ptr<GameOver> SharedPtr;
	typedef std::weak_ptr<Game> GameWeakPtr;
	typedef std::shared_ptr<Game> GameSharedPtr;

	GameOver(const GameWeakPtr& InGame);
	void Input(const InputHandler& Handler, const float Delta) override;
	void Render() override;

private:
	void HandleInput() const;
	void RenderSelectArrows(const glm::vec2& OptionPos, const glm::vec2& OptionSize) const;

	GameSharedPtr GetGame() const;

	GameWeakPtr GamePtr;
};