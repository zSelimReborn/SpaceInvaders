#pragma once

#include <string>
#include <glm/gtc/matrix_transform.hpp>

struct ProjectileData
{
	float InitialLifeSpan;
	float Speed;
	glm::vec3 Size;
	glm::vec3 Direction;

	ProjectileData() : InitialLifeSpan(0.f), Speed(0.f), Size(1.f) {}
	ProjectileData(float InLifeSpan, float InSpeed,
		const glm::vec3& InSize, const glm::vec3& InDirection
	)
		: InitialLifeSpan(InLifeSpan), Speed(InSpeed), Size(InSize), Direction(InDirection) {}

	void Fill(float InLifeSpan, float InSpeed,
		const glm::vec3& InSize, const glm::vec3& InDirection)
	{
		InitialLifeSpan = InLifeSpan;
		Speed = InSpeed;
		Size = InSize;
		Direction = InDirection;
	}
};
