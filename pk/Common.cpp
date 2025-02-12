#include "Common.h"

#include <iostream>

void Console::Print(const glm::vec3& Vector)
{
	std::cout << "{x: " << Vector.x << ", y: " << Vector.y << ", z: " << Vector.z << "}\n";
}

float Math::Clamp(const float Value, const float Min, const float Max)
{
	return std::min(Max, std::max(Min, Value));
}

Transform::Transform()
	: Location(0.f), Size(0.f)
{
}

Transform::Transform(const glm::vec3& _Location, const glm::vec3& _Size)
	: Location(_Location), Size(_Size)
{
}

BoundingBox::BoundingBox(const Transform& _Transform)
	: mTransform(_Transform)
{
	ScaleOffset.x = mTransform.Size.x / 2.0f;
	ScaleOffset.y = mTransform.Size.y / 2.0f;
}

float BoundingBox::Top() const
{
	return mTransform.Location.y + ScaleOffset.y;
}

float BoundingBox::Bottom() const
{
	return mTransform.Location.y - ScaleOffset.y;
}

float BoundingBox::Right() const
{
	return mTransform.Location.x + ScaleOffset.x;
}

float BoundingBox::Left() const
{
	return mTransform.Location.x - ScaleOffset.x;
}
