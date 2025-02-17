#include "Game.h"

#include "Assets.h"
#include "Ship.h"
#include "pk/AssetManager.h"

using namespace Assets;

Game::Game(const Window::WeakPtr& InWindow)
	: Scene(InWindow)
{
	
}

void Game::Initialize(Transform ShipInitialTransform, float ShipSpeed, float ShipCooldown, const ProjectileData& ShipProjectileInfo)
{
	PlayerShip = std::make_shared<Ship>(ShipInitialTransform, ShipSpeed);
	PlayerShip->SetColor(Colors::White);
	PlayerShip->SetShootCooldown(ShipCooldown);
	PlayerShip->SetProjectileData(ShipProjectileInfo);

	Add(PlayerShip);
}

void Game::Begin()
{
	Shader::SharedPtr ShapeShader = AssetManager::Get().LoadShader(Shaders::ShapeName, Shaders::ShapeVertexFile, Shaders::ShapeFragmentFile);
	ShapeShader->Use();
	ShapeShader->SetMatrix("projection", GetProjection());

	PlayerShip->SetShader(Shaders::ShapeName);
}

