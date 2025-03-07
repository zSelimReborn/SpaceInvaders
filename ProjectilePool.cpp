#include "ProjectilePool.h"

#include "Assets.h"
#include "Effects.h"
#include "Ship.h"

#include "pk/core/vfx/Emitter.h"
#include "pk/core/utils/ClassSettingsReader.h"

const int ProjectilePool::DEFAULT_POOL_SIZE = 10;
const float ProjectilePool::DEFAULT_LIFE_SPAN = 0.f;
const float ProjectilePool::DEFAULT_SPEED = 500.f;
const float ProjectilePool::DEFAULT_PARTICLE_SPEED = 200.f;
const float ProjectilePool::DEFAULT_PARTICLE_LIFE = 0.5f;
const float ProjectilePool::DEFAULT_PARTICLE_SCALE = 5.f;
const glm::vec3 ProjectilePool::DEFAULT_SIZE = glm::vec3(5.f, 10.f, 1.f);
const glm::vec3 ProjectilePool::DEFAULT_DIRECTION = glm::vec3(0.f, 1.f, 0.f);
const glm::vec4 ProjectilePool::DEFAULT_PARTICLE_COLOR = Colors::Red;

ProjectilePool::ProjectilePool(std::string InConfigFile)
	: ConfigFile(std::move(InConfigFile)), PoolSize(0), NextIndex(0),
		ParticleSpeed(DEFAULT_PARTICLE_SPEED), ParticleLife(DEFAULT_PARTICLE_LIFE),
		ParticleScale(DEFAULT_PARTICLE_SCALE), ParticleColor(DEFAULT_PARTICLE_COLOR)
{
	Pool.clear();
	SetDefaults();
	LoadConfig();
	CreatePool();
	PrepareEmitter();
}

Projectile::SharedPtr ProjectilePool::Create(const glm::vec3& InLocation, Team InTeam)
{
	Projectile::SharedPtr OutProjectile = Pool[NextIndex];
	OutProjectile->CancelDestroy();
	OutProjectile->SetLocation(InLocation);
	OutProjectile->SetTeam(InTeam);
	OutProjectile->SetSize(ProjectileInfo.Size);
	OutProjectile->SetInitialLifeSpan(ProjectileInfo.InitialLifeSpan);
	OutProjectile->SetShader(ShaderName);
	OutProjectile->SetTexture(TextureName);

	const glm::vec3 Velocity = ProjectileInfo.Direction * ProjectileInfo.Speed;
	OutProjectile->SetVelocity(Velocity);

	OutProjectile->AddOnHitDelegate([this](const Actor::SharedPtr& HitActor, const CollisionResult& Result)
		{
			OnProjectileHit(HitActor, Result);
		}
	);

	NextIndex = (NextIndex + 1) % PoolSize;
	return OutProjectile;
}

int ProjectilePool::GetPoolSize() const
{
	return PoolSize;
}

float ProjectilePool::GetLifeSpan() const
{
	return ProjectileInfo.InitialLifeSpan;
}

float ProjectilePool::GetSpeed() const
{
	return ProjectileInfo.Speed;
}

float ProjectilePool::GetParticleSpeed() const
{
	return ParticleSpeed;
}

float ProjectilePool::GetParticleLife() const
{
	return ParticleLife;
}

float ProjectilePool::GetParticleScale() const
{
	return ParticleLife;
}

glm::vec3 ProjectilePool::GetSize() const
{
	return ProjectileInfo.Size;
}

glm::vec3 ProjectilePool::GetDirection() const
{
	return ProjectileInfo.Direction;
}

glm::vec4 ProjectilePool::GetParticleColor() const
{
	return ParticleColor;
}

std::string ProjectilePool::GetShaderName() const
{
	return ShaderName;
}

std::string ProjectilePool::GetTextureName() const
{
	return TextureName;
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

void ProjectilePool::SetShaderName(const std::string& InShaderName)
{
	ShaderName = InShaderName;
}

void ProjectilePool::SetTextureName(const std::string& InTextureName)
{
	TextureName = InTextureName;
}

void ProjectilePool::ResetPool() const
{
	for (const Projectile::SharedPtr& Projectile : Pool)
	{
		Projectile->Destroy();
	}

	ExplosionEmitter->Reset();
}

void ProjectilePool::UpdateEffects(float Delta) const
{
	ExplosionEmitter->Update(Delta);
}

void ProjectilePool::RenderEffects() const
{
	ExplosionEmitter->Render();
}

void ProjectilePool::OnProjectileHit(const Actor::SharedPtr& HitActor, const CollisionResult& Result)
{
	if (HitActor->IsA<Ship>())
	{
		ExplosionEmitter->Spawn(Result.ImpactLocation, ParticleScale * 2.f);
	}
	else
	{
		ExplosionEmitter->Spawn(HitActor->GetLocation());
	}
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
	ClassSettings::SharedConstPtr PoolSettings = ClassSettingsReader::Load(ConfigFile);
	if (!PoolSettings)
	{
		return;
	}

	int InPoolSize;
	float InSpeed, InLifeSpan, InParticleSpeed, InParticleLife, InParticleScale;
	glm::vec3 InSize, InDirection;
	glm::vec4 InParticleColor;
	PoolSettings->Get("PoolSize", DEFAULT_POOL_SIZE, InPoolSize);
	PoolSettings->Get("ProjectileSpeed", DEFAULT_SPEED, InSpeed);
	PoolSettings->Get("ProjectileLifeSpan", DEFAULT_LIFE_SPAN, InLifeSpan);
	PoolSettings->Get("ProjectileSize", InSize);
	PoolSettings->Get("ProjectileDirection", InDirection);

	PoolSettings->Get("ParticleSpeed", DEFAULT_PARTICLE_SPEED, InParticleSpeed);
	PoolSettings->Get("ParticleLife", DEFAULT_PARTICLE_LIFE, InParticleLife);
	PoolSettings->Get("ParticleScale", DEFAULT_PARTICLE_SCALE, InParticleScale);
	PoolSettings->Get("ParticleColor", InParticleColor);

	SetPoolSize(InPoolSize);
	SetLifeSpan(InLifeSpan);
	SetSpeed(InSpeed);
	SetSize(InSize);
	SetDirection(InDirection);
	SetParticleSpeed(InParticleSpeed);
	SetParticleLife(InParticleLife);
	SetParticleScale(InParticleScale);
	SetParticleColor(InParticleColor);
}

void ProjectilePool::CreatePool()
{
	for (int i = 0; i < PoolSize; ++i)
	{
		Projectile::SharedPtr NewProjectile = std::make_shared<Projectile>();
		NewProjectile->SetColor(Colors::White);
		Pool.push_back(NewProjectile);
	}
}

void ProjectilePool::PrepareEmitter()
{
	constexpr int SpawnAmount = 1;
	constexpr int ParticlePoolCapacity = SpawnAmount * 5;

	ParticlePattern::Base::SharedPtr FirePattern = std::make_shared<SpawnTexture>(ParticleSpeed, ParticleLife, SpawnAmount, Colors::White);
	ExplosionEmitter = std::make_shared<Emitter>(ParticlePoolCapacity, ParticleScale, Assets::Shaders::ParticleTextureName, Assets::Textures::ExplosionName, FirePattern);
}

void ProjectilePool::SetParticleSpeed(float InSpeed)
{
	ParticleSpeed = std::abs(InSpeed);
}

void ProjectilePool::SetParticleLife(float InLife)
{
	ParticleLife = std::abs(InLife);
}

void ProjectilePool::SetParticleScale(float InScale)
{
	ParticleScale = std::abs(InScale);
}

void ProjectilePool::SetParticleColor(const glm::vec4& InColor)
{
	ParticleColor = glm::abs(InColor);
}
