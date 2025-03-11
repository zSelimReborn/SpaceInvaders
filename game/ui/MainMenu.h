#pragma once

#include "../../pk/ui/Widget.h"

using namespace pk;

class Game;
class GameSave;

enum class Panel : std::uint8_t
{
	Main,
	Scores
};

class MainMenu : public Widget
{
public:
	typedef std::shared_ptr<MainMenu> SharedPtr;
	typedef std::weak_ptr<Game> GameWeakPtr;
	typedef std::shared_ptr<Game> GameSharedPtr;

	static const int START_GAME_OPTION;
	static const int SCORES_OPTION;
	static const int MUTE_OPTION;
	static const int QUIT_OPTION;
	static const int MAX_OPTION;

	MainMenu(const GameWeakPtr& InGame);

	void Input(const InputHandler& Handler, const float Delta) override;
	void Render() override;

private:
	GameSharedPtr GetGame() const;

	void RenderMain() const;
	void RenderScores() const;

	void OnChangeChoice() const;
	void HandleChoice();
	void StartGame();
	void ShowScores();
	void ToggleMute() const;
	void QuitGame();
	void BackMenu();

	void PlayNavSound() const;

	void RenderSelectArrows(const std::string& InText, const glm::vec2& OptionPos, const glm::vec2& OptionSize) const;

	int CurrentChoice;
	int MaxChoice;

	GameWeakPtr GamePtr;

	Panel CurrentPanel;
};