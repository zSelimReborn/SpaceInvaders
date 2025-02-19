#pragma once

#include "pk/Scene.h"

class Ship;
class ProjectilePool;

class Game : public Scene
{
public:
	typedef std::shared_ptr<Game> SharedPtr;
	typedef std::shared_ptr<Ship> ShipSharedPtr;
	typedef std::shared_ptr<ProjectilePool> ProjectilePoolPtr;

	static const glm::vec3 DEFAULT_SHIP_SIZE;

	Game(const Window::WeakPtr& InWindow);

	void Begin() override;

private:
	void LoadConfig();
	void SpawnPlayer();

	glm::vec3 ShipSize;

	ShipSharedPtr PlayerShip;

	ProjectilePoolPtr PlayerProjectilePool;
};