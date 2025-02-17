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

	Game(const Window::WeakPtr& InWindow);
	void Initialize(Transform ShipInitialTransform, float ShipSpeed, float ShipCooldown, const ProjectileData& ShipProjectileInfo);

	void Begin() override;

private:
	ShipSharedPtr PlayerShip;
};