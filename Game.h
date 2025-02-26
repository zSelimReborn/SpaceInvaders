#pragma once

#include "pk/Scene.h"

class Ship;
class ProjectilePool;
class AlienGroup;
class Secret;
class Bunker;

class Game : public Scene
{
public:
	typedef std::shared_ptr<Game> SharedPtr;
	typedef std::shared_ptr<Ship> ShipSharedPtr;
	typedef std::shared_ptr<ProjectilePool> ProjectilePoolPtr;
	typedef std::shared_ptr<AlienGroup> AlienGroupPtr;
	typedef std::shared_ptr<Secret> SecretAlienPtr;
	typedef std::shared_ptr<Bunker> BunkerPtr;
	typedef std::vector<BunkerPtr> BunkerList;

	static const glm::vec3 DEFAULT_SHIP_SIZE;
	static const int MAX_NUM_BUNKERS;
	static const int DEFAULT_NUM_BUNKERS;
	static const float DEFAULT_BUNKERS_BOTTOM_OFFSET;

	Game(const Window::WeakPtr& InWindow);

	void Begin() override;

private:
	void LoadConfig();
	void SpawnPlayer();
	void SpawnAliens();
	void SpawnSecretAlien();
	void SpawnBunkers();

	glm::vec3 GetPlayerStartLocation() const;
	void BuildBunkers() const;
	void ResetPlayer() const;

	void OnInvadersReachedPlayer();
	void OnInvadersDefeat();
	void OnPlayerTakeDamage();

	void GameOver(bool bPlayerWon) const;

	void SetNumBunkers(int InNumBunkers);
	void SetShipSize(const glm::vec3& InSize);
	void SetBunkersBottomOffset(float InOffset);

	int NumBunkers;
	float BunkersBottomOffset;
	glm::vec3 ShipSize;

	ShipSharedPtr PlayerShip;
	AlienGroupPtr MainAlienGroup;
	SecretAlienPtr SecretAlien;
	BunkerList Bunkers;

	ProjectilePoolPtr PlayerProjectilePool;
	ProjectilePoolPtr AlienProjectilePool;
};