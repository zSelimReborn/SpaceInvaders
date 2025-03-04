#include "Common.h"

#include <fstream>
#include <iostream>
#include <sstream>

void Console::Print(const glm::vec3& Vector)
{
	std::cout << "{x: " << Vector.x << ", y: " << Vector.y << ", z: " << Vector.z << "}\n";
}

float Math::Clamp(const float Value, const float Min, const float Max)
{
	return std::min(Max, std::max(Min, Value));
}

float Math::Lerp(const float Min, const float Max, const float Ratio)
{
	return Min * (1.0 - Ratio) + (Max * Ratio);
}

bool File::Exists(const std::string& FilePath)
{
	std::ifstream Handler(FilePath);
	return Handler.good();
}

void File::Create(const std::string& FilePath, std::ios_base::openmode Mode)
{
	std::ofstream Handler(FilePath, Mode);
}

std::string File::ReadAll(const std::string& FilePath)
{
	std::ifstream Handler(FilePath);
	std::stringstream Stream;
	Handler.exceptions(std::ifstream::badbit);
	Handler.open(FilePath);
	Stream << Handler.rdbuf();

	return Stream.str();
}

void File::WriteAll(const std::string& FilePath, const std::string& FileContent)
{
	std::ofstream Handler(FilePath);
	Handler << FileContent;
}


Transform::Transform()
	: Location(0.f), Size(0.f)
{
}

Transform::Transform(const glm::vec3& InLocation, const glm::vec3& InSize)
	: Location(InLocation), Size(InSize)
{
}

BoundingBox::BoundingBox(const Transform& InTransform)
	: mTransform(InTransform)
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
