#pragma once

#include "pk/Scene.h"

class Ship;
class ProjectilePool;
class AlienGroup;
class Secret;
class Bunker;
class Hud;
class MainMenu;
class GameOver;
class GameSave;

enum class GameState : std::uint8_t
{
	Play,
	Menu,
	Pause,
	GameOver
};

class Game : public Scene
{
public:
	typedef std::shared_ptr<Game> SharedPtr;
	typedef std::shared_ptr<Ship> ShipSharedPtr;
	typedef std::shared_ptr<ProjectilePool> ProjectilePoolPtr;
	typedef std::shared_ptr<AlienGroup> AlienGroupPtr;
	typedef std::shared_ptr<Secret> SecretAlienPtr;
	typedef std::shared_ptr<Bunker> BunkerPtr;
	typedef std::shared_ptr<Hud> HudPtr;
	typedef std::shared_ptr<MainMenu> MainMenuPtr;
	typedef std::shared_ptr<GameOver> GameOverPtr;
	typedef std::shared_ptr<GameSave> GameSavePtr;
	typedef std::vector<BunkerPtr> BunkerList;

	static const glm::vec3 DEFAULT_SHIP_SIZE;
	static const int MAX_NUM_BUNKERS;
	static const int DEFAULT_NUM_BUNKERS;
	static const float DEFAULT_BUNKERS_BOTTOM_OFFSET;
	static const int DEFAULT_TEXT_SIZE;
	static const float DEFAULT_PLAYER_HIT_COOLDOWN;

	Game(const Window::WeakPtr& InWindow);

	void Begin() override;

	void Play();
	void Menu();
	void Quit() const;
	void ToggleMute();
	bool IsMuted() const;

	int PlayAudio(const std::string& Name, float Volume) const;
	int PlayAudio(const std::string& Name, float Volume, bool bLoop) const;

protected:
	void HandleInput(const float Delta) override;
	void Update(const float Delta) override;
	void Render(const float Delta) override;

private:
	void LoadAssets() const;

	void LoadSave();
	void WriteSave() const;

	void LoadConfig();
	void SpawnPlayer();
	void SpawnAliens();
	void SpawnSecretAlien();
	void SpawnBunkers();
	void ConstructMainMenu();
	void ConstructHud();
	void ConstructGameOver();

	glm::vec3 GetPlayerStartLocation() const;
	void BuildBunkers() const;
	void ResetPlayer() const;

	void OnInvadersReachedPlayer();
	void OnInvadersDefeat();
	void OnPlayerTakeDamage();

	void OnGameOver(bool bPlayerWon);

	void SetNumBunkers(int InNumBunkers);
	void SetTextSize(int InSize);
	void SetShipSize(const glm::vec3& InSize);
	void SetBunkersBottomOffset(float InOffset);
	void SetPlayerHitCooldown(float InCooldown);

	void UpdatePlayerHitCooldown(float Delta);

	int NumBunkers;
	int TextSize;
	unsigned int MainAudioChannel;
	float BunkersBottomOffset;
	glm::vec3 ShipSize;

	float CurrentHitCooldown;
	float PlayerHitCooldown;

	ShipSharedPtr PlayerShip;
	AlienGroupPtr MainAlienGroup;
	SecretAlienPtr SecretAlien;
	BunkerList Bunkers;

	HudPtr MainHud;
	MainMenuPtr MainMenuW;
	GameOverPtr GameOverW;

	ProjectilePoolPtr PlayerProjectilePool;
	ProjectilePoolPtr AlienProjectilePool;

	GameState State;

	GameSavePtr SavePtr;
};