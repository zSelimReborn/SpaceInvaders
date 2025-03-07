#pragma once

#include <memory>
#include <stdexcept>
#include <string>

class Texture
{
public:
	typedef std::shared_ptr<Texture> SharedPtr;

	Texture(std::string InPath, int InFormat, int InWrapS, int InWrapT, int InMinFilter, int InMaxFilter);

	unsigned int GetId() const;
	std::string GetPath() const;
	int GetWidth() const;
	int GetHeight() const;

	void Bind() const;
	void UnBind() const;

	class LoadError : public std::runtime_error
	{
		using std::runtime_error::runtime_error;
	};

private:
	unsigned int Id;
	std::string Path;

	int Width;
	int Height;
	int Channels;
	int Format;
	int WrapS;
	int WrapT;
	int MinFilter;
	int MaxFilter;
};