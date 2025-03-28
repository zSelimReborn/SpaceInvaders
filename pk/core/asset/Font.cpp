#include "Font.h"
#include "Shader.h"

#include <glad/glad.h>

#include "AssetManager.h"
#include "../render/Renderer.h"

using namespace pk;

Font::Font(std::string InPath, std::string InName, std::string InTextShader)
	: Path(std::move(InPath)), Name(std::move(InName)), TextShader(std::move(InTextShader)), Size(14)
{
}

std::string Font::GetName() const
{
	return Name;
}

std::string Font::GetPath() const
{
	return Path;
}

std::string Font::GetShaderName() const
{
    return TextShader;
}

Shader::SharedPtr Font::GetShader() const
{
    return AssetManager::Get().GetShader(TextShader);
}

unsigned int Font::GetSize() const
{
    return Size;
}

void Font::Load(unsigned int InSize, int InWrapMode, int InFilterMode)
{
    Characters.clear();

    FT_Library FontLibrary;
    if (FT_Init_FreeType(&FontLibrary))
    {
        throw LoadError("ERROR::FREETYPE: Could not init FreeType Library");
    }

    FT_Face FontFace;
    if (FT_New_Face(FontLibrary, Path.c_str(), 0, &FontFace))
    {
        throw LoadError("ERROR::FREETYPE: Failed to load font");
    }

    Size = InSize;
    FT_Set_Pixel_Sizes(FontFace, 0, Size);
    LoadCharacters(FontFace, InWrapMode, InFilterMode);

    FT_Done_Face(FontFace);
    FT_Done_FreeType(FontLibrary);

    bLoaded = true;
}

void Font::Render(const std::string& Text, const glm::vec2& Position, float Scale, const glm::vec4& Color) const
{
    Renderer::Get().RenderText(Text, GetShader(), Characters, Position, Scale, Color);
}

void Font::GetTextSize(const std::string& Text, float Scale, float& OutHSize, float& OutVSize) const
{
    OutHSize = 0;
    OutVSize = 0;

    float LastAdvance = 0;
    for (const char c : Text)
    {
        Character Glyph = Characters.at(c);

        float h = Glyph.Size.y * Scale;
        OutVSize = std::max(OutVSize, h);
        LastAdvance = (Glyph.Advance >> 6) * Scale;
        OutHSize += LastAdvance;
    }
}

float Font::GetCharacterAdvance(char InCharacter, float Scale) const
{
    Character Glyph = Characters.at(InCharacter);
    return (Glyph.Advance >> 6) * Scale;
}

Font::CharacterMap Font::GetCharacterMap() const
{
    return Characters;
}

void Font::LoadCharacters(FT_Face& Face, int InWrapMode, int InFilterMode)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(Face, c, FT_LOAD_RENDER))
        {
            continue;
        }

        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            Face->glyph->bitmap.width,
            Face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            Face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, InWrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, InWrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, InFilterMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, InFilterMode);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(Face->glyph->bitmap.width, Face->glyph->bitmap.rows),
            glm::ivec2(Face->glyph->bitmap_left, Face->glyph->bitmap_top),
            Face->glyph->advance.x
        };

        Characters.insert(std::pair<char, Character>(c, character));
    }
}

