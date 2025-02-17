#pragma once

#include <string>
#include <glm/gtc/matrix_transform.hpp>

struct ProjectileData
{
	float InitialLifeSpan;
	float Speed;
	glm::vec3 SpawnOffset;
	glm::vec3 Size;
	glm::vec3 Direction;
	std::string Shader;

	ProjectileData() : InitialLifeSpan(0.f), Speed(0.f), SpawnOffset(0.f), Size(1.f), Shader("") {};
	ProjectileData(float InLifeSpan, float InSpeed, 
		const glm::vec3& InSpawnOffset, const glm::vec3& InSize, const glm::vec3& InDirection, 
		const std::string& InShader
	)
		: InitialLifeSpan(InLifeSpan), Speed(InSpeed),
		SpawnOffset(InSpawnOffset), Size(InSize), Direction(InDirection),
		Shader(InShader) {}			
};
