#pragma once

#include "../../pk/core/vfx/Emitter.h"

class Explosion : public ParticlePattern::Base
{
public:
	Explosion(float InSpeed, float InLife, int InSpawnAmount, const glm::vec4& InColor);
	void Spawn(ParticleList& Pool, int& LastInactive, const glm::vec3& Position, const glm::vec3& Direction, float OverrideScale) override;

	~Explosion() override = default;
private:
	void BuildCompass();

	std::vector<glm::vec3> Compass;
};

class SpawnTexture : public ParticlePattern::Base
{
public:
	SpawnTexture(float InSpeed, float InLife, int InSpawnAmount, const glm::vec4& InColor);
	void Spawn(ParticleList& Pool, int& LastInactive, const glm::vec3& Position, const glm::vec3& Direction, float OverrideScale) override;

	~SpawnTexture() override = default;
};