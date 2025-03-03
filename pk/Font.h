#pragma once

#include <string>
#include <stdexcept>
#include <map>
#include <memory>

#include "Shader.h"

#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
	unsigned int TextureID;  // ID handle of the glyph texture
	glm::ivec2   Size;       // Size of glyph
	glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Offset to advance to next glyph
};

class Font
{
public:
	typedef std::shared_ptr<Font> SharedPtr;
	typedef std::map<char, Character> CharacterMap;

	Font(std::string InPath, std::string InName, std::string InTextShader);

	std::string GetName() const;
	std::string GetPath() const;
	std::string GetShaderName() const;
	Shader::SharedPtr GetShader() const;
	unsigned int GetSize() const;

	void Load(unsigned int InSize, int InWrapMode, int InFilterMode);
	void Render(const std::string& Text, const glm::vec2& Position, float Scale, const glm::vec4& Color) const;
	void GetTextSize(const std::string& Text, float Scale, float& OutHSize, float& OutVSize);

	CharacterMap GetCharacterMap() const;

	class LoadError : public std::runtime_error
	{
		using std::runtime_error::runtime_error;
	};

private:
	void LoadCharacters(FT_Face& Face, int InWrapMode, int InFilterMode);

	std::string Path;
	std::string Name;
	std::string TextShader;
	unsigned int Size;

	CharacterMap Characters;

	glm::mat4 Projection;

	bool bLoaded = false;
};