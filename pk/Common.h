#pragma once

#include <glm/glm.hpp>

namespace Console
{
	void Print(const glm::vec3& Vector);
}

namespace Math
{
	float Clamp(const float Value, const float Min, const float Max);
}

namespace Colors
{
	const glm::vec4 White(1.f, 1.f, 1.f, 1.f);
	const glm::vec4 Black(0.f, 0.f, 0.f, 1.f);
	const glm::vec4 Red(1.f, 0.f, 0.f, 1.f);
	const glm::vec4 Green(0.f, 1.f, 0.f, 1.f);
	const glm::vec4 LightBlack(0.14f, 0.15f, 0.15f, 1.f);
}