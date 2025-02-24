#pragma once

#include "pk/Actor.h"
#include "pk/IDamageable.h"

class ProjectilePool;
class TeamComponent;

class Ship : public Actor, public IDamageable
{
public:
	typedef std::shared_ptr<ProjectilePool> ProjectilePoolPtr;
	typedef std::shared_ptr<TeamComponent> TeamComponentPtr;

	static const float DEFAULT_SPEED;
	static const float DEFAULT_COOLDOWN;

	Ship(const Transform& InTransform);
	Ship(const glm::vec3& InLocation, const glm::vec3& InSize);

	void SetSpeed(float InSpeed);
	float GetSpeed() const;
	float GetMaxSpeed() const;

	void SetShootCooldown(float InCooldown);
	float GetShootCooldown() const;

	void SetProjectilePool(const ProjectilePoolPtr& InProjectilePool);

	void LoadConfig() override;
	void Input(const InputHandler& Handler, const float Delta) override;
	void Update(const float Delta) override;

	bool TakeDamage(float InDamage) override;

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

	TeamComponentPtr TeamPtr;

	ProjectilePoolPtr CurrentProjectilePool;
};
