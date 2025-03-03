#include "AssetManager.h"

Shader::SharedPtr AssetManager::LoadShader(const std::string& Name, const std::string& Vertex,
	const std::string& Fragment)
{
	Shader::SharedPtr FoundShader = GetShader(Name);
	if (FoundShader != nullptr)
	{
		return FoundShader;
	}

	Shader::SharedPtr NewShader = std::make_shared<Shader>();
	NewShader->Compile(Vertex, Fragment);
	Shaders.insert(ShaderPair(Name, NewShader));

	return NewShader;
}

Texture::SharedPtr AssetManager::LoadTexture(const std::string& Name, const std::string& Path, int InFormat, int InWrapS, int InWrapT, int InMinFilter, int InMaxFilter)
{
	Texture::SharedPtr FoundTexture = GetTexture(Name);
	if (FoundTexture != nullptr)
	{
		return FoundTexture;
	}

	Texture::SharedPtr NewTexture = std::make_shared<Texture>(Path, InFormat, InWrapS, InWrapT, InMinFilter, InMaxFilter);
	Textures.insert(TexturePair(Name, NewTexture));

	return NewTexture;
}

Font::SharedPtr AssetManager::LoadFont(const std::string& Name, const std::string& Path, const std::string& ShaderName)
{
	Font::SharedPtr FoundFont = GetFont(Name);
	if (FoundFont != nullptr)
	{
		return FoundFont;
	}

	Shader::SharedPtr FoundShader = GetShader(ShaderName);
	Font::SharedPtr NewFont = std::make_shared<Font>(Path, Name, ShaderName);
	Fonts.insert(FontPair(Name, NewFont));

	return NewFont;
}

Texture::SharedPtr AssetManager::GetTexture(const std::string& Name)
{
	if (Textures.count(Name) <= 0)
	{
		return nullptr;
	}

	return Textures[Name];
}

Font::SharedPtr AssetManager::GetFont(const std::string& Name)
{
	if (Fonts.count(Name) <= 0)
	{
		return nullptr;
	}

	return Fonts[Name];
}

Shader::SharedPtr AssetManager::GetShader(const std::string& Name)
{
	if (Shaders.count(Name) <= 0)
	{
		return nullptr;
	}

	return Shaders[Name];
}
