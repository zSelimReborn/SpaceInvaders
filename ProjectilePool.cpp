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

	const glm::vec3 Velocity = ProjectileInfo.Direction * ProjectileInfo.Speed;
	OutProjectile->SetVelocity(Velocity);

	OutProjectile->SetShader(InShaderName);

	NextIndex = (NextIndex + 1) % PoolSize;
	return OutProjectile;
}

int ProjectilePool::GetPoolSize() const
{
}

float ProjectilePool::GetLifeSpan() const
{
	return ProjectileInfo.InitialLifeSpan;
}

float ProjectilePool::GetSpeed() const
{
	return ProjectileInfo.Speed;
}

glm::vec3 ProjectilePool::GetSize() const
{
	return ProjectileInfo.Size;
}

glm::vec3 ProjectilePool::GetDirection() const
{
	return ProjectileInfo.Direction;
}

void ProjectilePool::SetLifeSpan(float InLifeSpan)
{
	ProjectileInfo.InitialLifeSpan = std::max(0.f, InLifeSpan);
}

void ProjectilePool::SetSpeed(float InSpeed)
{
	ProjectileInfo.Speed = std::max(0.f, InSpeed);
}

void ProjectilePool::SetSize(const glm::vec3& InSize)
{
	ProjectileInfo.Size = glm::abs(InSize);
}

void ProjectilePool::SetDirection(const glm::vec3& InDirection)
{
	ProjectileInfo.Direction = glm::normalize(InDirection);
}

void ProjectilePool::SetPoolSize(int InPoolSize)
{
	PoolSize = std::max(DEFAULT_POOL_SIZE, InPoolSize);
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

	int InPoolSize;
	float InSpeed, InLifeSpan;
	glm::vec3 InSize, InDirection;
	PoolSettings->Get("PoolSize", DEFAULT_POOL_SIZE, InPoolSize);
	PoolSettings->Get("ProjectileSpeed", DEFAULT_SPEED, InSpeed);
	PoolSettings->Get("ProjectileLifeSpan", DEFAULT_LIFE_SPAN, InLifeSpan);
	PoolSettings->Get("ProjectileSize", InSize);
	PoolSettings->Get("ProjectileDirection", InDirection);

	SetPoolSize(InPoolSize);
	SetLifeSpan(InLifeSpan);
	SetSpeed(InSpeed);
	SetSize(InSize);
	SetDirection(InDirection);
}

void ProjectilePool::CreatePool()
{
	for (int i = 0; i < PoolSize; ++i)
	{
		Projectile::SharedPtr NewProjectile = std::make_shared<Projectile>();
		Pool.push_back(NewProjectile);
	}
}
