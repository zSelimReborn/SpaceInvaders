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
	static const glm::vec3 DEFAULT_ALIEN_SIZE;

	Game(const Window::WeakPtr& InWindow);

	void Begin() override;

private:
	void LoadConfig();
	void SpawnPlayer();
	void SpawnAliens();

	glm::vec3 ShipSize;
	glm::vec3 AlienSize;

	ShipSharedPtr PlayerShip;

	ProjectilePoolPtr PlayerProjectilePool;
	ProjectilePoolPtr AlienProjectilePool;
};