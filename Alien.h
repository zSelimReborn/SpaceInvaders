#pragma once

#include "pk/Actor.h"

class ProjectilePool;

enum class AlienType : uint8_t
{
	Squid,
	Crab,
	Octopus
};

class Alien : public Actor
{
public:
	typedef std::shared_ptr<Alien> SharedPtr;

	Alien(AlienType InType);
	Alien(const Transform& InTransform, AlienType InType);
	Alien(const glm::vec3& InLocation, const glm::vec3& InSize, AlienType InType);

	void SetProjectilePool(const std::shared_ptr<ProjectilePool>& InProjectilePool);

	int GetScore() const;
	AlienType GetType() const;

	void LoadConfig() override;

private:
	int Score;
	AlienType Type;

	std::shared_ptr<ProjectilePool> ProjectilePoolPtr;
};