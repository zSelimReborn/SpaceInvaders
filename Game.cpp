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

using namespace Assets;

const glm::vec3 Game::DEFAULT_SHIP_SIZE = glm::vec3(75.f, 20.f, 1.f);
const int Game::MAX_NUM_BUNKERS = 6;
const int Game::DEFAULT_NUM_BUNKERS = 3;
const float Game::DEFAULT_BUNKERS_BOTTOM_OFFSET = 90.f;

Game::Game(const Window::WeakPtr& InWindow)
	: Scene(InWindow),
		NumBunkers(DEFAULT_NUM_BUNKERS), BunkersBottomOffset(DEFAULT_BUNKERS_BOTTOM_OFFSET),
		ShipSize(DEFAULT_SHIP_SIZE)
{
	LoadConfig();

	IHandler.HandleKey(GLFW_KEY_LEFT, InputType::Hold);
	IHandler.HandleKey(GLFW_KEY_RIGHT, InputType::Hold);
	IHandler.HandleKey(GLFW_KEY_SPACE, InputType::Press);
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

	int InNumBunkers;
	float InBunkersBottomOffset;
	glm::vec3 InShipSize(DEFAULT_SHIP_SIZE);
	GameSettings->Get("ShipSize", InShipSize);
	GameSettings->Get("NumBunkers", DEFAULT_NUM_BUNKERS, InNumBunkers);
	GameSettings->Get("BunkersBottomOffset", DEFAULT_BUNKERS_BOTTOM_OFFSET, InBunkersBottomOffset);

	SetNumBunkers(InNumBunkers);
	SetBunkersBottomOffset(InBunkersBottomOffset);
	SetShipSize(InShipSize);
}

void Game::SpawnPlayer()
{
	const float Width = static_cast<float>(GetScreenWidth());
	const float Height = static_cast<float>(GetScreenHeight());

	const glm::vec3 ShipSizeHalf(ShipSize.x / 2.f, ShipSize.y / 2.f, 1.f);
	const glm::vec3 ShipBaseLocation(Width / 2, Height - ShipSizeHalf.y, 1.0f);
	const Transform ShipTransform(ShipBaseLocation, ShipSize);

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

void Game::OnPlayerTakeDamage()
{
	// TODO Add hit animation
	// TODO Handle game over
	std::cout << "Player LP: " << PlayerShip->GetLifePoints() << "\n";
	if (PlayerShip->GetLifePoints() <= 0)
	{
		std::cout << "GAME OVER\n";
	}
}

void Game::SetNumBunkers(int InNumBunkers)
{
	InNumBunkers = std::abs(InNumBunkers);
	NumBunkers = std::min(InNumBunkers, MAX_NUM_BUNKERS);
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
	Shader::SharedPtr ShapeShader = AssetManager::Get().LoadShader(Shaders::ShapeName, Shaders::ShapeVertexFile, Shaders::ShapeFragmentFile);
	ShapeShader->Use();
	ShapeShader->SetMatrix("projection", GetProjection());

	PlayerProjectilePool = std::make_shared<ProjectilePool>(Config::PlayerProjectilePool);
	AlienProjectilePool = std::make_shared<ProjectilePool>(Config::AlienProjectilePool);

	SpawnPlayer();
	SpawnAliens();
	SpawnSecretAlien();
	SpawnBunkers();

	Scene::Begin();
}

