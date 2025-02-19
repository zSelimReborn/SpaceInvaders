#pragma once

#include "pk/Actor.h"

class ProjectilePool;

class Alien : public Actor
{
public:
	typedef std::shared_ptr<Alien> SharedPtr;

	Alien(const Transform& InTransform);
	Alien(const glm::vec3& InLocation, const glm::vec3& InSize);

	void SetProjectilePool(const std::shared_ptr<ProjectilePool>& InProjectilePool);

	int GetScore() const;

	void LoadConfig() override;

private:
	int Score;

	std::shared_ptr<ProjectilePool> ProjectilePoolPtr;
};