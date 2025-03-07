#pragma once

#include <string>

#include "../actors/Projectile.h"
#include "../Types.h"

class Emitter;

class ProjectilePool
{
public:
	static const int DEFAULT_POOL_SIZE;
	static const float DEFAULT_LIFE_SPAN;
	static const float DEFAULT_SPEED;
	static const float DEFAULT_PARTICLE_SPEED;
	static const float DEFAULT_PARTICLE_LIFE;
	static const float DEFAULT_PARTICLE_SCALE;
	static const glm::vec3 DEFAULT_SIZE;
	static const glm::vec3 DEFAULT_DIRECTION;
	static const glm::vec4 DEFAULT_PARTICLE_COLOR;

	typedef std::vector<Projectile::SharedPtr> ProjectileList;
	typedef std::shared_ptr<Emitter> EmitterPtr;

	ProjectilePool(std::string InConfigFile);
	Projectile::SharedPtr Create(const glm::vec3& InLocation, Team InTeam);

	int GetPoolSize() const;
	float GetLifeSpan() const;
	float GetSpeed() const;
	float GetParticleSpeed() const;
	float GetParticleLife() const;
	float GetParticleScale() const;
	glm::vec3 GetSize() const;
	glm::vec3 GetDirection() const;
	glm::vec4 GetParticleColor() const;
	std::string GetShaderName() const;
	std::string GetTextureName() const;

	void SetLifeSpan(float InLifeSpan);
	void SetSpeed(float InSpeed);
	void SetSize(const glm::vec3& InSize);
	void SetDirection(const glm::vec3& InDirection);
	void SetShaderName(const std::string& InShaderName);
	void SetTextureName(const std::string& InTextureName);

	void ResetPool() const;

	void UpdateEffects(float Delta) const;
	void RenderEffects() const;

private:
	void OnProjectileHit(const Actor::SharedPtr& HitActor, const CollisionResult& Result);

	void SetPoolSize(int InPoolSize);
	void SetDefaults();
	void LoadConfig();
	void CreatePool();
	void PrepareEmitter();

	void SetParticleSpeed(float InSpeed);
	void SetParticleLife(float InLife);
	void SetParticleScale(float InScale);
	void SetParticleColor(const glm::vec4& InColor);

	std::string ConfigFile;
	std::string ShaderName;
	std::string TextureName;

	ProjectileList Pool;
	int PoolSize;
	int NextIndex;

	float ParticleSpeed;
	float ParticleLife;
	float ParticleScale;
	glm::vec4 ParticleColor;

	ProjectileData ProjectileInfo;

	EmitterPtr ExplosionEmitter;
};
