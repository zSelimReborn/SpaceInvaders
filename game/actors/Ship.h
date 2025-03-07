#pragma once

#include <functional>

#include "../../pk/core/world/Actor.h"
#include "../../pk/core/interfaces/IDamageable.h"

class ProjectilePool;
class TeamComponent;
class Game;

class Ship : public Actor, public IDamageable
{
public:
	typedef std::shared_ptr<ProjectilePool> ProjectilePoolPtr;
	typedef std::shared_ptr<TeamComponent> TeamComponentPtr;
	typedef std::shared_ptr<Game> GameSharedPtr;
	typedef std::weak_ptr<Game> GameWeakPtr;
	typedef std::function<void()> OnTakeDamageDelegate;

	static const float DEFAULT_SPEED;
	static const float DEFAULT_COOLDOWN;
	static const int DEFAULT_LIFE_POINTS;

	Ship(const Transform& InTransform);
	Ship(const glm::vec3& InLocation, const glm::vec3& InSize);

	void SetMaxSpeed(float InSpeed);
	void SetSpeed(float InSpeed);
	void SetShootCooldown(float InCooldown);
	void SetMaxLifePoints(int InLifePoints);
	void SetLifePoints(int InLifePoints);

	float GetSpeed() const;
	float GetMaxSpeed() const;
	float GetShootCooldown() const;
	int GetMaxLifePoints() const;
	int GetLifePoints() const;
	int GetScorePoints() const;

	void ResetLifePoints();
	void ResetScorePoints();

	void SetProjectilePool(const ProjectilePoolPtr& InProjectilePool);

	void LoadConfig() override;
	void Begin() override;
	void Input(const InputHandler& Handler, const float Delta) override;
	void Update(const float Delta) override;

	bool TakeDamage(float InDamage) override;

	void AddOnTakeDamageObserver(const OnTakeDamageDelegate& Callback);

private:
	void ConstraintInViewport(const float Delta);
	void Shoot();
	void UpdateCooldown(const float Delta);

	void OnProjectileHit(const Actor::SharedPtr& HitActor);
	void OnProjectileDestroy();

	void NotifyOnTakeDamage() const;

	void PlayAudio(const std::string& Name, const std::string& FilePath, float Volume) const;

	GameSharedPtr GetGame() const;

	bool bCanShoot;
	float MaxSpeed;
	float Speed;
	float ShootCooldown;
	float CurrentCooldown;
	bool bShouldCooldown;
	int MaxLifePoints;
	int LifePoints;
	int ScorePoints;

	TeamComponentPtr TeamPtr;

	ProjectilePoolPtr CurrentProjectilePool;

	std::vector<OnTakeDamageDelegate> OnTakeDamageFunctions;

	GameWeakPtr GamePtr;
};
