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
	Projectile::SharedPtr Create(const glm::vec3& InLocation, const std::string& InShaderName);

private:
	void SetDefaults();
	void LoadConfig();
	void CreatePool();

	std::string ConfigFile;
	ProjectileList Pool;
	int PoolSize;
	int NextIndex;

	ProjectileData ProjectileInfo;
};
