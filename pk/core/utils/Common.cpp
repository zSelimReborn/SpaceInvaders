#include "Common.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <Windows.h>
#elif __linux__
#include <sys/stat.h> 
#include <sys/types.h>
#endif

using namespace pk;

void Console::Print(const glm::vec3& Vector)
{
	std::cout << "{x: " << Vector.x << ", y: " << Vector.y << ", z: " << Vector.z << "}\n";
}

float Math::Clamp(const float Value, const float Left, const float Right)
{
	return Min(Right, Math::Max(Left, Value));
}

int Math::Clamp(int Value, int Left, int Right)
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

int Math::Min(int Left, int Right)
{
	return (Left < Right) ? Left : Right;
}

float Math::Max(float Left, float Right)
{
	return (Left > Right) ? Left : Right;
}

int Math::Max(int Left, int Right)
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


std::string String::ToLower(const std::string& InString)
{
	std::string Lowered = InString;
	std::transform(Lowered.begin(), Lowered.end(), Lowered.begin(), [](unsigned char Char) { return tolower(Char); });

	return Lowered;
}

std::string String::ToUpper(const std::string& InString)
{
	std::string Uppered = InString;
	std::transform(Uppered.begin(), Uppered.end(), Uppered.begin(), [](unsigned char Char) { return toupper(Char); });

	return Uppered;
}

std::vector<std::string> String::GenerateStringsFromBase(const std::string& InBase, int Size)
{
	std::vector<std::string> Strings;
	const int BufferSize = static_cast<int>(InBase.size()) + 2;
	char* FileName = new char[BufferSize];
	for (int i = 0; i < Size; ++i)
	{
		if (sprintf_s(FileName, BufferSize, InBase.c_str(), i + 1) <= 0)
		{
			continue;
		}

		Strings.emplace_back(FileName);
	}

	delete[] FileName;
	return Strings;
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
	return mTransform.Location.y - ScaleOffset.y;
}

float BoundingBox::Bottom() const
{
	return mTransform.Location.y + ScaleOffset.y;
}

float BoundingBox::Right() const
{
	return mTransform.Location.x + ScaleOffset.x;
}

float BoundingBox::Left() const
{
	return mTransform.Location.x - ScaleOffset.x;
}
