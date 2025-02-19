#pragma once

#include "pk/Scene.h"

class Ship;
class ProjectilePool;
class AlienGroup;

class Game : public Scene
{
public:
	typedef std::shared_ptr<Game> SharedPtr;
	typedef std::shared_ptr<Ship> ShipSharedPtr;
	typedef std::shared_ptr<ProjectilePool> ProjectilePoolPtr;
	typedef std::shared_ptr<AlienGroup> AlienGroupPtr;

	static const glm::vec3 DEFAULT_SHIP_SIZE;

	Game(const Window::WeakPtr& InWindow);

	void Begin() override;

private:
	void LoadConfig();
	void SpawnPlayer();
	void SpawnAliens();

	glm::vec3 ShipSize;

	ShipSharedPtr PlayerShip;
	AlienGroupPtr MainAlienGroup;

	ProjectilePoolPtr PlayerProjectilePool;
	ProjectilePoolPtr AlienProjectilePool;
};