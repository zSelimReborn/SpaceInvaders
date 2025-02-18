#pragma once

#include "pk/Actor.h"
#include "Types.h"

class Ship : public Actor
{
public:
	static const float DEFAULT_SPEED;
	static const float DEFAULT_COOLDOWN;
	static const glm::vec3 DEFAULT_PROJECTILE_SIZE;
	static const float DEFAULT_PROJECTILE_SPEED;

	Ship(const Transform& InTransform);
	Ship(const glm::vec3& InLocation, const glm::vec3& InSize);

	void SetSpeed(float InSpeed);
	float GetSpeed() const;
	float GetMaxSpeed() const;

	void SetShootCooldown(float InCooldown);
	float GetShootCooldown() const;

	void SetProjectileData(const ProjectileData& InProjectileInfo);

	void LoadConfig() override;
	void Input(const Window& Window, const float Delta) override;
	void Update(const float Delta) override;

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

	ProjectileData ProjectileInfo;
};
