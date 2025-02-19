#include "Game.h"

#include <iostream>

#include "Alien.h"
#include "Assets.h"
#include "ProjectilePool.h"
#include "Ship.h"
#include "pk/AssetManager.h"
#include "pk/SettingsReader.h"

using namespace Assets;

const glm::vec3 Game::DEFAULT_SHIP_SIZE = glm::vec3(75.f, 20.f, 1.f);
const glm::vec3 Game::DEFAULT_ALIEN_SIZE = glm::vec3(10.f, 10.f, 1.f);

Game::Game(const Window::WeakPtr& InWindow)
	: Scene(InWindow), ShipSize(DEFAULT_SHIP_SIZE), AlienSize(DEFAULT_ALIEN_SIZE)
{
	LoadConfig();
}

void Game::LoadConfig()
{
	Settings::SharedConstPtr GameSettings = SettingsReader::Load(Config::GameFile);
	if (GameSettings == nullptr)
	{
		std::cout << "[Game] - Unable to read config file.\n";
		return;
	}

	GameSettings->Get("ShipSize", ShipSize);
	GameSettings->Get("AlienSize", AlienSize);
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

	Add(PlayerShip);
}

void Game::SpawnAliens()
{
	const glm::vec2 Center(GetScreenCenter());
	const glm::vec3 SpawnLocation(Center.x, Center.y, 1.f);

	Alien::SharedPtr TempAlien = std::make_shared<Alien>(SpawnLocation, AlienSize);
	TempAlien->SetConfig(Config::SquidFile);
	TempAlien->SetProjectilePool(AlienProjectilePool);
	TempAlien->SetShader(Shaders::ShapeName);

	Add(TempAlien);
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
	AddPendingActors();

	Scene::Begin();
}

