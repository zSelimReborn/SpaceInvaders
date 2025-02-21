#pragma once

#include "pk/Actor.h"
#include "ITeamDefiner.h"

class ProjectilePool;

class Ship : public Actor, public ITeamDefiner
{
public:
	static const float DEFAULT_SPEED;
	static const float DEFAULT_COOLDOWN;

	Ship(const Transform& InTransform);
	Ship(const glm::vec3& InLocation, const glm::vec3& InSize);

	void SetSpeed(float InSpeed);
	float GetSpeed() const;
	float GetMaxSpeed() const;

	void SetShootCooldown(float InCooldown);
	float GetShootCooldown() const;

	void SetProjectilePool(const std::shared_ptr<ProjectilePool>& InProjectilePool);

	void LoadConfig() override;
	void Input(const Window& Window, const float Delta) override;
	void Update(const float Delta) override;

	void SetTeam(Team InTeam) override;
	Team GetTeam() const override;

private:
	void ConstraintInViewport(const float Delta);
	void Shoot();
	void UpdateCooldown(const float Delta);

	void OnProjectileHit(const Actor::SharedPtr& HitActor);

	float MaxSpeed;
	float Speed;
	float ShootCooldown;
	float CurrentCooldown;
	bool bShouldCooldown;

	Team CurrentTeam;

	std::shared_ptr<ProjectilePool> ProjectilePoolPtr;
};
