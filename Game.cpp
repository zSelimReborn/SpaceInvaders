#include "Game.h"

#include <iostream>
#include <glad/glad.h>

#include "AlienGroup.h"
#include "Assets.h"
#include "ProjectilePool.h"
#include "Secret.h"
#include "Ship.h"
#include "pk/AssetManager.h"
#include "pk/ClassSettingsReader.h"

#include <GLFW/glfw3.h>

#include "Bunker.h"
#include "GameOver.h"
#include "GameSave.h"
#include "Hud.h"
#include "MainMenu.h"
#include "pk/ISound.h"
#include "pk/Random.h"
#include "pk/SaveSystem.h"
#include "pk/SoundEngine.h"

using namespace Assets;

const glm::vec3 Game::DEFAULT_SHIP_SIZE = glm::vec3(75.f, 20.f, 1.f);
const int Game::MAX_NUM_BUNKERS = 6;
const int Game::DEFAULT_NUM_BUNKERS = 3;
const float Game::DEFAULT_BUNKERS_BOTTOM_OFFSET = 90.f;
const int Game::DEFAULT_TEXT_SIZE = 26;
const float Game::DEFAULT_PLAYER_HIT_COOLDOWN = 2.f;

Game::Game()
	: NumBunkers(DEFAULT_NUM_BUNKERS), TextSize(DEFAULT_TEXT_SIZE), MainAudioChannel(0),
	  BunkersBottomOffset(DEFAULT_BUNKERS_BOTTOM_OFFSET),
	  ShipSize(DEFAULT_SHIP_SIZE),
	  CurrentHitCooldown(0.f), PlayerHitCooldown(DEFAULT_PLAYER_HIT_COOLDOWN),
	  State(GameState::Play)
{
	LoadConfig();
	IHandler.HandlePad(GLFW_JOYSTICK_1);

	IHandler.HandleKey(GLFW_KEY_LEFT, InputType::Hold);
	IHandler.HandleKey(GLFW_KEY_RIGHT, InputType::Hold);
	IHandler.HandleKey(GLFW_KEY_SPACE, InputType::Hold);

	IHandler.HandleKey(GLFW_KEY_UP, InputType::Press);
	IHandler.HandleKey(GLFW_KEY_DOWN, InputType::Press);
	IHandler.HandleKey(GLFW_KEY_ENTER, InputType::Press);

	IHandler.HandlePadKey(GLFW_GAMEPAD_BUTTON_START, InputType::Press);
	IHandler.HandlePadKey(GLFW_GAMEPAD_BUTTON_DPAD_UP, InputType::Press);
	IHandler.HandlePadKey(GLFW_GAMEPAD_BUTTON_DPAD_DOWN, InputType::Press);
	IHandler.HandlePadKey(GLFW_GAMEPAD_BUTTON_CROSS, InputType::Press);

	IHandler.HandlePadKey(GLFW_GAMEPAD_BUTTON_DPAD_LEFT, InputType::Hold);
	IHandler.HandlePadKey(GLFW_GAMEPAD_BUTTON_DPAD_RIGHT, InputType::Hold);
	IHandler.HandlePadKey(GLFW_GAMEPAD_BUTTON_SQUARE, InputType::Hold);
}

Game::Game(Window::WeakPtr InWindow)
	: Game()
{
	SetWindow(std::move(InWindow));
}

void Game::LoadConfig()
{
	ClassSettings::SharedConstPtr GameSettings = ClassSettingsReader::Load(Config::GameFile);
	if (GameSettings == nullptr)
	{
		std::cout << "[Game] - Unable to read config file.\n";
		return;
	}

	int InNumBunkers, InTextSize;
	float InBunkersBottomOffset, InPlayerHitCooldown;
	glm::vec3 InShipSize(DEFAULT_SHIP_SIZE);
	GameSettings->Get("ShipSize", InShipSize);
	GameSettings->Get("NumBunkers", DEFAULT_NUM_BUNKERS, InNumBunkers);
	GameSettings->Get("TextSize", DEFAULT_TEXT_SIZE, InTextSize);
	GameSettings->Get("BunkersBottomOffset", DEFAULT_BUNKERS_BOTTOM_OFFSET, InBunkersBottomOffset);
	GameSettings->Get("PlayerHitCooldown", DEFAULT_PLAYER_HIT_COOLDOWN, InPlayerHitCooldown);

	SetNumBunkers(InNumBunkers);
	SetTextSize(InTextSize);
	SetBunkersBottomOffset(InBunkersBottomOffset);
	SetShipSize(InShipSize);
	SetPlayerHitCooldown(InPlayerHitCooldown);
}

void Game::SpawnPlayer()
{
	PlayerProjectilePool = std::make_shared<ProjectilePool>(Config::PlayerProjectilePool);
	PlayerProjectilePool->SetShaderName(Shaders::SpriteNoColorName);
	PlayerProjectilePool->SetTextureName(Textures::BlueLaserName);

	const glm::vec3 StartLocation = GetPlayerStartLocation();
	const Transform ShipTransform(StartLocation, ShipSize);

	PlayerShip = std::make_shared<Ship>(ShipTransform);
	PlayerShip->SetConfig(Config::PlayerFile);
	PlayerShip->SetProjectilePool(PlayerProjectilePool);
	PlayerShip->SetShader(Shaders::SpriteName);
	PlayerShip->SetTexture(Textures::PlayerName);
	PlayerShip->AddOnTakeDamageObserver([this]() { OnPlayerTakeDamage(); });

	Add(PlayerShip);
}

void Game::SpawnAliens()
{
	AlienProjectilePool = std::make_shared<ProjectilePool>(Config::AlienProjectilePool);
	AlienProjectilePool->SetShaderName(Shaders::SpriteNoColorName);
	AlienProjectilePool->SetTextureName(Textures::RedLaserName);

	MainAlienGroup = std::make_shared<AlienGroup>();
	MainAlienGroup->SetConfig(Config::AlienGroupFile);
	MainAlienGroup->SetProjectilePool(AlienProjectilePool);
	MainAlienGroup->AddOnReachedPlayerDelegate([this]() { OnInvadersReachedPlayer(); });
	MainAlienGroup->AddOnDefeatDelegate([this]() { OnInvadersDefeat(); });

	Add(MainAlienGroup);
}

void Game::SpawnSecretAlien()
{
	SecretAlien = std::make_shared<Secret>();
	SecretAlien->SetConfig(Config::SecretAlienFile);

	Add(SecretAlien);
}

void Game::SpawnBunkers()
{
	const float Width = static_cast<float>(GetScreenWidth());
	const float Height = static_cast<float>(GetScreenHeight());

	const float SingleBunkerWidth = Width / static_cast<float>(NumBunkers);
	float BunkerX = SingleBunkerWidth / 2;

	const float BunkerY = Height - (ShipSize.y) - BunkersBottomOffset;

	for (int i = 0; i < NumBunkers; i++)
	{
		const glm::vec3 BunkerLocation(BunkerX, BunkerY, 1.f);
		Bunker::SharedPtr CurrentBunker = std::make_shared<Bunker>(BunkerLocation, glm::vec3(0.f));
		CurrentBunker->SetConfig(Config::BunkerFile);
		Bunkers.push_back(CurrentBunker);
		Add(CurrentBunker);

		BunkerX += SingleBunkerWidth;
	}
}

void Game::ConstructMainMenu()
{
	const std::weak_ptr<Game> GameWeak = std::dynamic_pointer_cast<Game>(shared_from_this());
	MainMenuW = std::make_shared<MainMenu>(GameWeak);
	Add(MainMenuW);
}

void Game::ConstructHud()
{
	MainHud = std::make_shared<Hud>(Fonts::TextFontName);
	Add(MainHud);
}

void Game::ConstructGameOver()
{
	const std::weak_ptr<Game> GameWeak = std::dynamic_pointer_cast<Game>(shared_from_this());
	GameOverW = std::make_shared<GameOver>(GameWeak);
	Add(GameOverW);
}

glm::vec3 Game::GetPlayerStartLocation() const
{
	const float Width = static_cast<float>(GetScreenWidth());
	const float Height = static_cast<float>(GetScreenHeight());

	const glm::vec3 ShipSizeHalf(ShipSize.x / 2.f, ShipSize.y / 2.f, 1.f);
	const glm::vec3 StartLocation(Width / 2, Height - ShipSizeHalf.y, 1.0f);
	return StartLocation;
}

void Game::BuildBunkers() const
{
	for (const Bunker::SharedPtr& Bunker : Bunkers)
	{
		Bunker->Build();
	}
}

void Game::ResetPlayer() const
{
	PlayerShip->SetLocation(GetPlayerStartLocation());
	PlayerShip->ResetLifePoints();
	PlayerShip->ResetScorePoints();
}

void Game::OnInvadersReachedPlayer()
{
	OnGameOver(false);
}

void Game::OnInvadersDefeat()
{
	OnGameOver(true);
}

void Game::OnPlayerTakeDamage()
{
	if (PlayerShip->GetLifePoints() <= 0)
	{
		OnGameOver(false);
	}
	else
	{
		SecretAlien->Reset();
		PlayAudio(Sounds::PlayerExplosionName, 1.f);
		State = GameState::Pause;
	}
}

void Game::OnGameOver(bool bPlayerWon)
{
	if (!bPlayerWon)
	{
		SaveScore();
		PlayAudio(Sounds::GameOverName, 1.f);
		State = GameState::GameOver;
		MainAlienGroup->HideBoard();
		AlienProjectilePool->ResetPool();
		PlayerProjectilePool->ResetPool();
		MainMenuW->Deactivate();
		GameOverW->Activate();
	}
	else
	{
		MainAlienGroup->StartGroup();
	}
}

void Game::SetNumBunkers(int InNumBunkers)
{
	InNumBunkers = std::abs(InNumBunkers);
	NumBunkers = std::min(InNumBunkers, MAX_NUM_BUNKERS);
}

void Game::SetTextSize(int InSize)
{
	TextSize = std::abs(InSize);
}

void Game::SetShipSize(const glm::vec3& InSize)
{
	ShipSize = glm::abs(InSize);
}

void Game::SetBunkersBottomOffset(float InOffset)
{
	BunkersBottomOffset = std::abs(InOffset);
}

void Game::SetPlayerHitCooldown(float InCooldown)
{
	PlayerHitCooldown = std::abs(InCooldown);
}

void Game::UpdatePlayerHitCooldown(float Delta)
{
	if (State != GameState::Pause)
	{
		return;
	}

	CurrentHitCooldown += Delta;
	if (CurrentHitCooldown >= PlayerHitCooldown)
	{
		CurrentHitCooldown = 0.f;
		AlienProjectilePool->ResetPool();
		PlayerProjectilePool->ResetPool();
		PlayerShip->SetLocation(GetPlayerStartLocation());

		State = GameState::Play;
	}
}

void Game::SaveScore() const
{
	if (SavePtr == nullptr || PlayerShip->GetScorePoints() <= 0)
	{
		return;
	}

	SavePtr->AddScore(PlayerShip->GetScorePoints());
}

void Game::Begin()
{
	LoadAssets();

	LoadSave();

	SpawnPlayer();
	SpawnBunkers();
	SpawnSecretAlien();
	SpawnAliens();
	ConstructMainMenu();
	ConstructHud();
	ConstructGameOver();

	Scene::Begin();

	Menu();
}

void Game::LoadSave()
{
	try
	{
		File::CreateFolder(Saves::BasePath);
		SaveSystem::Get().TriggerLoad<GameSave>(Saves::GameFile);
		SavePtr = SaveSystem::Get().GetSaveObject<GameSave>();
	}
	catch (const std::exception& Exception)
	{
		std::cout << "Failed to load save file.\n";
	}
}

void Game::WriteSave() const
{
	try
	{
		SaveSystem::Get().TriggerSave();
	}
	catch (const std::exception& Exception)
	{
		std::cout << "Failed to write save file.\n";
	}
}

void Game::Play()
{
	if (State == GameState::Play)
	{
		return;
	}

	State = GameState::Play;

	MainMenuW->Deactivate();
	GameOverW->Deactivate();
	MainHud->Activate();

	MainAlienGroup->StartGroup();
	AlienProjectilePool->ResetPool();
	PlayerProjectilePool->ResetPool();
	SecretAlien->Reset();
	BuildBunkers();
	ResetPlayer();

	SoundEngine::Get().Stop(MainAudioChannel);
}

void Game::Menu()
{
	if (State == GameState::Menu)
	{
		return;
	}

	State = GameState::Menu;

	AlienProjectilePool->ResetPool();
	PlayerProjectilePool->ResetPool();
	SecretAlien->Reset();

	MainHud->Deactivate();
	GameOverW->Deactivate();
	MainMenuW->Activate();

	MainAudioChannel = PlayAudio(Sounds::MainJingleName, 1.f, true);

	SaveScore();
}

void Game::Quit()
{
	WriteSave();
	Scene::Quit();
}

void Game::ToggleMute()
{
	if (SavePtr == nullptr)
	{
		return;
	}

	SavePtr->ToggleMute();
	SoundEngine::Get().Mute(MainAudioChannel, SavePtr->IsMuted());
}

bool Game::IsMuted() const
{
	if (SavePtr == nullptr)
	{
		return false;
	}

	return SavePtr->IsMuted();
}

int Game::GetMaxScores() const
{
	if (SavePtr == nullptr)
	{
		return 0;
	}

	return SavePtr->GetMaxScores();
}

std::vector<int> Game::GetHighScores() const
{
	std::vector<int> Empty;
	if (SavePtr == nullptr)
	{
		return Empty;
	}

	return SavePtr->GetHighScores();
}

int Game::PlayAudio(const std::string& Name, float Volume) const
{
	return PlayAudio(Name, Volume, false);
}

int Game::PlayAudio(const std::string& Name, float Volume, bool bLoop) const
{
	bool bMuted = false;
	if (SavePtr != nullptr)
	{
		bMuted = SavePtr->IsMuted();
	}

	const AssetManager::SoundSharedPtr Sound = AssetManager::Get().GetSound(Name);
	if (Sound == nullptr)
	{
		return 0;
	}

	return SoundEngine::Get().Play(Sound->GetPath(), Volume, bMuted, bLoop);
}

void Game::HandleInput(const float Delta)
{
	if (IHandler.IsPressed(GLFW_KEY_ESCAPE) || IHandler.IsPadPressed(GLFW_GAMEPAD_BUTTON_START))
	{
		Menu();
	}

	if (State == GameState::Play)
	{
		HandleActorsInput(Delta);
	}
	else
	{
		HandleWidgetInput(Delta);
	}
}

void Game::Update(const float Delta)
{
	MainHud->SetLifePoints(PlayerShip->GetLifePoints());
	MainHud->SetScore(PlayerShip->GetScorePoints());

	if (State == GameState::Play)
	{
		Scene::Update(Delta);

		PlayerProjectilePool->UpdateEffects(Delta);
		AlienProjectilePool->UpdateEffects(Delta);
	}

	UpdatePlayerHitCooldown(Delta);
}

void Game::Render(const float Delta)
{
	if (State != GameState::Menu)
	{
		RenderActors();
		PlayerProjectilePool->RenderEffects();
		AlienProjectilePool->RenderEffects();
	}

	RenderWidgets();
}

void Game::LoadAssets() const
{
	AssetManager::Get().LoadTexture(Textures::PlayerName, Textures::PlayerPath, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST);
	AssetManager::Get().LoadTexture(Textures::SquidName, Textures::SquidPath, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST);
	AssetManager::Get().LoadTexture(Textures::CrabName, Textures::CrabPath, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST);
	AssetManager::Get().LoadTexture(Textures::OctopusName, Textures::OctopusPath, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST);
	AssetManager::Get().LoadTexture(Textures::SecretName, Textures::SecretPath, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST);
	AssetManager::Get().LoadTexture(Textures::ExplosionName, Textures::ExplosionPath, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST);
	AssetManager::Get().LoadTexture(Textures::BlueLaserName, Textures::BlueLaserPath, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST);
	AssetManager::Get().LoadTexture(Textures::RedLaserName, Textures::RedLaserPath, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST);

	Shader::SharedPtr ShapeShader = AssetManager::Get().LoadShader(Shaders::ShapeName, Shaders::ShapeVertexFile, Shaders::ShapeFragmentFile);
	ShapeShader->Use();
	ShapeShader->SetMatrix("projection", GetProjection());

	Shader::SharedPtr SpriteShader = AssetManager::Get().LoadShader(Shaders::SpriteName, Shaders::SpriteVertexFile, Shaders::SpriteFragmentFile);
	SpriteShader->Use();
	SpriteShader->SetMatrix("projection", GetProjection());

	Shader::SharedPtr SpriteNoColorShader = AssetManager::Get().LoadShader(Shaders::SpriteNoColorName, Shaders::SpriteVertexFile, Shaders::SpriteNoColorFragmentFile);
	SpriteNoColorShader->Use();
	SpriteNoColorShader->SetMatrix("projection", GetProjection());

	Shader::SharedPtr TextShader = AssetManager::Get().LoadShader(Shaders::TextName, Shaders::TextVertexFile, Shaders::TextFragmentFile);
	TextShader->Use();
	TextShader->SetMatrix("projection", GetProjection());

	Shader::SharedPtr ParticleShapeShader = AssetManager::Get().LoadShader(Shaders::ParticleShapeName, Shaders::ParticleVertexFile, Shaders::ParticleShapeFragmentFile);
	ParticleShapeShader->Use();
	ParticleShapeShader->SetMatrix("projection", GetProjection());

	Shader::SharedPtr ParticleTextureShader = AssetManager::Get().LoadShader(Shaders::ParticleTextureName, Shaders::ParticleVertexFile, Shaders::ParticleTextureFragmentFile);
	ParticleTextureShader->Use();
	ParticleTextureShader->SetMatrix("projection", GetProjection());

	Font::SharedPtr TextFont = AssetManager::Get().LoadFont(Fonts::TextFontName, Fonts::TextFontPath, Shaders::TextName);
	TextFont->Load(TextSize, GL_CLAMP_TO_BORDER, GL_NEAREST);

	Font::SharedPtr HeadingFont = AssetManager::Get().LoadFont(Fonts::HeadingFontName, Fonts::HeadingFontPath, Shaders::TextName);
	HeadingFont->Load(50, GL_CLAMP_TO_BORDER, GL_NEAREST);

	AssetManager::Get().LoadSound(Sounds::MenuNavigationName, Sounds::MenuNavigation);
	AssetManager::Get().LoadSound(Sounds::OldShootName, Sounds::OldShoot);
	AssetManager::Get().LoadSound(Sounds::AlienExplosionName, Sounds::AlienExplosion);
	AssetManager::Get().LoadSound(Sounds::PlayerExplosionName, Sounds::PlayerExplosion);
	AssetManager::Get().LoadSound(Sounds::GameOverName, Sounds::GameOver);
	AssetManager::Get().LoadSound(Sounds::MainJingleName, Sounds::MainJingle);
	AssetManager::Get().LoadSound(Sounds::SecretAlienSpawnName, Sounds::SecretAlienSpawn);
	AssetManager::Get().LoadSequenceSound(Sounds::AlienMoveName, String::GenerateStringsFromBase(Sounds::AlienMove, 4));
	AssetManager::Get().LoadRandomSound(Sounds::ShootName, String::GenerateStringsFromBase(Sounds::Shoot, 4));
}

