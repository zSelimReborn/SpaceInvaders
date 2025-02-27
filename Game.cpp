#include "Game.h"

#include <iostream>

#include "AlienGroup.h"
#include "Assets.h"
#include "ProjectilePool.h"
#include "Secret.h"
#include "Ship.h"
#include "pk/AssetManager.h"
#include "pk/SettingsReader.h"

#include <GLFW/glfw3.h>

#include "Bunker.h"
#include "GameOver.h"
#include "Hud.h"
#include "MainMenu.h"

using namespace Assets;

const glm::vec3 Game::DEFAULT_SHIP_SIZE = glm::vec3(75.f, 20.f, 1.f);
const int Game::MAX_NUM_BUNKERS = 6;
const int Game::DEFAULT_NUM_BUNKERS = 3;
const float Game::DEFAULT_BUNKERS_BOTTOM_OFFSET = 90.f;
const int Game::DEFAULT_TEXT_SIZE = 26;

Game::Game(const Window::WeakPtr& InWindow)
	: Scene(InWindow),
		NumBunkers(DEFAULT_NUM_BUNKERS), TextSize(DEFAULT_TEXT_SIZE),
		BunkersBottomOffset(DEFAULT_BUNKERS_BOTTOM_OFFSET),
		ShipSize(DEFAULT_SHIP_SIZE), State(GameState::Menu)
{
	LoadConfig();

	IHandler.HandleKey(GLFW_KEY_LEFT, InputType::Hold);
	IHandler.HandleKey(GLFW_KEY_RIGHT, InputType::Hold);
	IHandler.HandleKey(GLFW_KEY_SPACE, InputType::Hold);

	IHandler.HandleKey(GLFW_KEY_UP, InputType::Press);
	IHandler.HandleKey(GLFW_KEY_DOWN, InputType::Press);
	IHandler.HandleKey(GLFW_KEY_ENTER, InputType::Press);
}

void Game::LoadConfig()
{
	Settings::SharedConstPtr GameSettings = SettingsReader::Load(Config::GameFile);
	if (GameSettings == nullptr)
	{
		std::cout << "[Game] - Unable to read config file.\n";
		return;
	}

	int InNumBunkers, InTextSize;
	float InBunkersBottomOffset;
	glm::vec3 InShipSize(DEFAULT_SHIP_SIZE);
	GameSettings->Get("ShipSize", InShipSize);
	GameSettings->Get("NumBunkers", DEFAULT_NUM_BUNKERS, InNumBunkers);
	GameSettings->Get("TextSize", DEFAULT_TEXT_SIZE, InTextSize);
	GameSettings->Get("BunkersBottomOffset", DEFAULT_BUNKERS_BOTTOM_OFFSET, InBunkersBottomOffset);

	SetNumBunkers(InNumBunkers);
	SetTextSize(InTextSize);
	SetBunkersBottomOffset(InBunkersBottomOffset);
	SetShipSize(InShipSize);
}

void Game::SpawnPlayer()
{
	const glm::vec3 StartLocation = GetPlayerStartLocation();
	const Transform ShipTransform(StartLocation, ShipSize);

	PlayerShip = std::make_shared<Ship>(ShipTransform);
	PlayerShip->SetConfig(Config::PlayerFile);
	PlayerShip->SetProjectilePool(PlayerProjectilePool);
	PlayerShip->SetShader(Shaders::ShapeName);
	PlayerShip->AddOnTakeDamageObserver([this]() { OnPlayerTakeDamage(); });

	Add(PlayerShip);
}

void Game::SpawnAliens()
{
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
	// TODO Add hit animation
	if (PlayerShip->GetLifePoints() <= 0)
	{
		OnGameOver(false);
	}
}

void Game::OnGameOver(bool bPlayerWon)
{
	if (!bPlayerWon)
	{
		State = GameState::GameOver;
		MainAlienGroup->HideBoard();
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

void Game::Begin()
{
	LoadAssets();

	PlayerProjectilePool = std::make_shared<ProjectilePool>(Config::PlayerProjectilePool);
	AlienProjectilePool = std::make_shared<ProjectilePool>(Config::AlienProjectilePool);

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

void Game::Play()
{
	State = GameState::Play;

	MainMenuW->Deactivate();
	GameOverW->Deactivate();
	MainHud->Activate();

	MainAlienGroup->StartGroup();
	SecretAlien->Reset();
	BuildBunkers();
	ResetPlayer();
}

void Game::Menu()
{
	State = GameState::Menu;

	MainHud->Deactivate();
	GameOverW->Deactivate();
	MainMenuW->Activate();
}

void Game::Quit() const
{
	const Window::SharedPtr CurrentWindow = GetWindow();
	if (CurrentWindow == nullptr)
	{
		std::cout << "Unable to quit, empty window\n";
		return;
	}

	CurrentWindow->ShouldClose(true);
}

void Game::HandleInput(const float Delta)
{
	if (IHandler.IsPressed(GLFW_KEY_ESCAPE))
	{
		if (State == GameState::Menu)
		{
			Quit();
		}
		else
		{
			Menu();
		}
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
	if (State != GameState::Play)
	{
		return;
	}

	MainHud->SetLifePoints(PlayerShip->GetLifePoints());
	MainHud->SetScore(PlayerShip->GetScorePoints());

	Scene::Update(Delta);
}

void Game::Render(const float Delta)
{
	if (State != GameState::Menu)
	{
		RenderActors();
	}

	RenderWidgets();
}

void Game::LoadAssets() const
{
	Shader::SharedPtr ShapeShader = AssetManager::Get().LoadShader(Shaders::ShapeName, Shaders::ShapeVertexFile, Shaders::ShapeFragmentFile);
	ShapeShader->Use();
	ShapeShader->SetMatrix("projection", GetProjection());

	Shader::SharedPtr TextShader = AssetManager::Get().LoadShader(Shaders::TextName, Shaders::TextVertexFile, Shaders::TextFragmentFile);
	TextShader->Use();
	TextShader->SetMatrix("projection", GetProjection());

	Font::SharedPtr TextFont = AssetManager::Get().LoadFont(Fonts::TextFontName, Fonts::TextFontPath, Shaders::TextName, GetProjection());
	TextFont->Load(TextSize);

	Font::SharedPtr HeadingFont = AssetManager::Get().LoadFont(Fonts::HeadingFontName, Fonts::HeadingFontPath, Shaders::TextName, GetProjection());
	HeadingFont->Load(50);
}

