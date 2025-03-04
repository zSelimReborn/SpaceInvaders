#include "Common.h"

#include <fstream>
#include <iostream>
#include <sstream>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <Windows.h>
#elif __linux__
#include <sys/stat.h> 
#include <sys/types.h>
#endif

void Console::Print(const glm::vec3& Vector)
{
	std::cout << "{x: " << Vector.x << ", y: " << Vector.y << ", z: " << Vector.z << "}\n";
}

float Math::Clamp(const float Value, const float Left, const float Right)
{
	return Min(Right, Math::Max(Left, Value));
}

float Math::Lerp(const float Left, const float Right, const float Ratio)
{
	return Left * (1.0 - Ratio) + (Right * Ratio);
}

float Math::Min(float Left, float Right)
{
	return (Left < Right) ? Left : Right;
}

float Math::Max(float Left, float Right)
{
	return (Left > Right) ? Left : Right;
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

int File::CreateFolder(const std::string& FolderName)
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	if (CreateDirectoryA(FolderName.c_str(), nullptr) || GetLastError() == ERROR_ALREADY_EXISTS) {
		return 0;
	}

	return GetLastError();
#elif __linux__
	int result = mkdir(FolderName.c_str(), 0777);
	return result;
#endif
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
