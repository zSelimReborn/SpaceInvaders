#pragma once

#include "pk/Actor.h"
#include "pk/IDamageable.h"

class ProjectilePool;
class TeamComponent;

enum class AlienType : uint8_t
{
	Squid,
	Crab,
	Octopus,
	Secret
};

class Alien : public Actor, public IDamageable
{
public:
	typedef std::shared_ptr<Alien> SharedPtr;
	typedef std::shared_ptr<ProjectilePool> ProjectilePoolPtr;
	typedef std::shared_ptr<TeamComponent> TeamComponentPtr;

	Alien(AlienType InType);
	Alien(const Transform& InTransform, AlienType InType);
	Alien(const glm::vec3& InLocation, const glm::vec3& InSize, AlienType InType);

	void SetProjectilePool(const ProjectilePoolPtr& InProjectilePool);

	int GetScore() const;
	AlienType GetType() const;

	void LoadConfig() override;

	bool TakeDamage(float InDamage) override;

	void Shoot() const;

private:
	int Score;
	AlienType Type;

	TeamComponentPtr TeamPtr;

	ProjectilePoolPtr CurrentProjectilePool;
};