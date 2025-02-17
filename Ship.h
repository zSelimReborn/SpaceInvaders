#pragma once

#include "pk/Actor.h"
#include "Types.h"

class Ship : public Actor
{
public:
	Ship(const Transform& InTransform, float InMaxSpeed);
	Ship(const glm::vec3& InLocation, const glm::vec3& InSize, float InMaxSpeed);

	void SetSpeed(float InSpeed);
	float GetSpeed() const;
	float GetMaxSpeed() const;

	void SetShootCooldown(float InCooldown);
	float GetShootCooldown() const;

	void SetProjectileData(const ProjectileData& InProjectileInfo);

	void Input(const Window& Window, const float Delta) override;
	void Update(const float Delta) override;

private:
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
