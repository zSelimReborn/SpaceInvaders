#pragma once

#include <string>

#include "Projectile.h"
#include "Types.h"

class ProjectilePool
{
public:
	static const int DEFAULT_POOL_SIZE;
	static const float DEFAULT_LIFE_SPAN;
	static const float DEFAULT_SPEED;
	static const glm::vec3 DEFAULT_SIZE;
	static const glm::vec3 DEFAULT_DIRECTION;

	typedef std::vector<Projectile::SharedPtr> ProjectileList;

	ProjectilePool(const std::string& InConfigFile);
	Projectile::SharedPtr Create(const glm::vec3& InLocation, Team InTeam, const std::string& InShaderName);

	int GetPoolSize() const;
	float GetLifeSpan() const;
	float GetSpeed() const;
	glm::vec3 GetSize() const;
	glm::vec3 GetDirection() const;

	void SetLifeSpan(float InLifeSpan);
	void SetSpeed(float InSpeed);
	void SetSize(const glm::vec3& InSize);
	void SetDirection(const glm::vec3& InDirection);

	void ResetPool() const;

private:
	void SetPoolSize(int InPoolSize);
	void SetDefaults();
	void LoadConfig();
	void CreatePool();

	std::string ConfigFile;
	ProjectileList Pool;
	int PoolSize;
	int NextIndex;

	ProjectileData ProjectileInfo;
};
