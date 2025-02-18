#pragma once

#include "pk/Common.h"
#include "pk/Scene.h"
#include "Types.h"

class Ship;

class Game : public Scene
{
public:
	typedef std::shared_ptr<Game> SharedPtr;
	typedef std::shared_ptr<Ship> ShipSharedPtr;

	static const glm::vec3 DEFAULT_SHIP_SIZE;

	Game(const Window::WeakPtr& InWindow);

	void Begin() override;

private:
	void LoadConfig();
	void SpawnPlayer();

	glm::vec3 ShipSize;

	ShipSharedPtr PlayerShip;
};