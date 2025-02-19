#include "ProjectilePool.h"

#include "pk/SettingsReader.h"

const int ProjectilePool::DEFAULT_POOL_SIZE = 10;
const float ProjectilePool::DEFAULT_LIFE_SPAN = 0.f;
const float ProjectilePool::DEFAULT_SPEED = 500.f;
const glm::vec3 ProjectilePool::DEFAULT_SIZE = glm::vec3(5.f, 10.f, 1.f);
const glm::vec3 ProjectilePool::DEFAULT_DIRECTION = glm::vec3(0.f, 1.f, 0.f);

ProjectilePool::ProjectilePool(const std::string& InConfigFile)
	: ConfigFile(InConfigFile), PoolSize(0), NextIndex(0)
{
	Pool.clear();
	SetDefaults();
	LoadConfig();
	CreatePool();
}

Projectile::SharedPtr ProjectilePool::Create(const glm::vec3& InLocation, const std::string& InShaderName)
{
	Projectile::SharedPtr OutProjectile = Pool[NextIndex];
	OutProjectile->CancelDestroy();
	OutProjectile->SetLocation(InLocation);
	OutProjectile->SetSize(ProjectileInfo.Size);
	OutProjectile->SetInitialLifeSpan(ProjectileInfo.InitialLifeSpan);

	const glm::vec3 Velocity = glm::normalize(ProjectileInfo.Direction) * ProjectileInfo.Speed;
	OutProjectile->SetVelocity(Velocity);

	OutProjectile->SetShader(InShaderName);

	NextIndex = (NextIndex + 1) % PoolSize;
	return OutProjectile;
}

void ProjectilePool::SetDefaults()
{
	PoolSize = DEFAULT_POOL_SIZE;
	ProjectileInfo.Fill(DEFAULT_LIFE_SPAN, DEFAULT_SPEED, DEFAULT_SIZE, DEFAULT_DIRECTION);
}

void ProjectilePool::LoadConfig()
{
	Settings::SharedConstPtr PoolSettings = SettingsReader::Load(ConfigFile);
	if (!PoolSettings)
	{
		return;
	}

	PoolSettings->Get("PoolSize", DEFAULT_POOL_SIZE, PoolSize);
	PoolSettings->Get("ProjectileSpeed", DEFAULT_SPEED, ProjectileInfo.Speed);
	PoolSettings->Get("ProjectileSize", ProjectileInfo.Size);
	PoolSettings->Get("ProjectileDirection", ProjectileInfo.Direction);
}

void ProjectilePool::CreatePool()
{
	for (int i = 0; i < PoolSize; ++i)
	{
		Projectile::SharedPtr NewProjectile = std::make_shared<Projectile>();
		Pool.push_back(NewProjectile);
	}
}
