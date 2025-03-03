#pragma once

#include <memory>
#include <glm/glm.hpp>

namespace Console
{
	void Print(const glm::vec3& Vector);
}

namespace Math
{
	float Clamp(const float Value, const float Min, const float Max);
	float Lerp(const float Min, const float Max, const float Ratio);
}

namespace Colors
{
	const glm::vec4 White(1.f, 1.f, 1.f, 1.f);
	const glm::vec4 Black(0.f, 0.f, 0.f, 1.f);
	const glm::vec4 Red(1.f, 0.f, 0.f, 1.f);
	const glm::vec4 Green(0.f, 1.f, 0.f, 1.f);
	const glm::vec4 LightBlack(0.14f, 0.15f, 0.15f, 1.f);
}

struct Transform
{
	glm::vec3 Location;
	glm::vec3 Size;

	Transform();
	Transform(const glm::vec3& _Location, const glm::vec3& _Size);
};

struct BoundingBox
{
	Transform mTransform;
	glm::vec2 ScaleOffset;

	BoundingBox(const Transform& _Transform);
	float Top() const;
	float Bottom() const;
	float Right() const;
	float Left() const;
};

struct CollisionResult
{
	bool bHit;
	glm::vec3 ImpactLocation;
};
