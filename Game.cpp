#include "Game.h"

#include "Assets.h"
#include "Ship.h"
#include "pk/AssetManager.h"
#include "pk/SettingsReader.h"

using namespace Assets;

const glm::vec3 Game::DEFAULT_SHIP_SIZE = glm::vec3(75.f, 20.f, 1.f);

Game::Game(const Window::WeakPtr& InWindow)
	: Scene(InWindow)
{
	LoadConfig();
}

void Game::LoadConfig()
{
	ShipSize = DEFAULT_SHIP_SIZE;

	Settings::SharedConstPtr GameSettings = SettingsReader::Load(Config::GameFile);
	if (GameSettings != nullptr)
	{
		GameSettings->Get("ShipSize", ShipSize);
	}
}

void Game::SpawnPlayer()
{
	const Window::SharedPtr Window = GetWindow();
	const float Width = static_cast<float>(Window->GetWidth());
	const float Height = static_cast<float>(Window->GetHeight());

	const glm::vec3 ShipSizeHalf(ShipSize.x / 2.f, ShipSize.y / 2.f, 1.f);
	const glm::vec3 ShipBaseLocation(Width / 2, Height - ShipSizeHalf.y, 1.0f);
	const Transform ShipTransform(ShipBaseLocation, ShipSize);

	PlayerShip = std::make_shared<Ship>(ShipTransform);
	PlayerShip->SetConfig(Config::PlayerFile);
	PlayerShip->SetShader(Shaders::ShapeName);

	Add(PlayerShip);
}

void Game::Begin()
{
	Shader::SharedPtr ShapeShader = AssetManager::Get().LoadShader(Shaders::ShapeName, Shaders::ShapeVertexFile, Shaders::ShapeFragmentFile);
	ShapeShader->Use();
	ShapeShader->SetMatrix("projection", GetProjection());

	SpawnPlayer();
	AddPendingActors();

	Scene::Begin();
}

