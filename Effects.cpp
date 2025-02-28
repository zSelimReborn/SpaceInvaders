#include "Effects.h"

Explosion::Explosion(float InSpeed, float InLife, int InSpawnAmount, const glm::vec4& InColor)
	: Base(false, InSpeed, InLife, InSpawnAmount, InColor)
{
	BuildCompass();
}

void Explosion::Spawn(ParticleList& Pool, int& LastInactive, const glm::vec3& Position, const glm::vec3& Direction)
{
	for (int i = 0; i < GetSpawnAmount(); ++i)
	{
		LastInactive = NextInactive(Pool, LastInactive);
		SpawnParticle(Pool[LastInactive], Position, Compass[i % Compass.size()]);
	}
}

void Explosion::BuildCompass()
{
	Compass = {
		glm::vec3(-1.f, -1.f, 0.f), // TopLeft
		glm::vec3(0.f, -1.f, 0.f), // Top
		glm::vec3(1.f, -1.f, 0.f), // TopRight
		glm::vec3(1.f, 0.f, 0.f), // Right
		glm::vec3(1.f, 1.f, 0.f), // BottomRight
		glm::vec3(0.f, 1.f, 0.f), // Bottom
		glm::vec3(-1.f, 1.f, 0.f), // BottomLeft
		glm::vec3(-1.f, 0.f, 0.f), // Left
	};
}
