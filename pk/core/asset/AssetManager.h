#pragma once

#include <map>
#include <string>
#include <vector>

#include "Shader.h"
#include "Texture.h"
#include "Font.h"

class ISound;

class AssetManager
{
public:
	typedef std::map<std::string, Shader::SharedPtr> ShaderMap;
	typedef std::pair<std::string, Shader::SharedPtr> ShaderPair;

	typedef std::map<std::string, Texture::SharedPtr> TextureMap;
	typedef std::pair<std::string, Texture::SharedPtr> TexturePair;

	typedef std::map<std::string, Font::SharedPtr> FontMap;
	typedef std::pair<std::string, Font::SharedPtr> FontPair;

	typedef std::shared_ptr<ISound> SoundSharedPtr;
	typedef std::map<std::string, SoundSharedPtr> SoundMap;
	typedef std::pair<std::string, SoundSharedPtr> SoundPair;

	static AssetManager& Get()
	{
		static AssetManager Instance;
		return Instance;
	}

	Shader::SharedPtr LoadShader(const std::string& Name, const std::string& Vertex, const std::string& Fragment);
	Texture::SharedPtr LoadTexture(const std::string& Name, const std::string& Path, int InFormat, int InWrapS, int InWrapT, int InMinFilter, int InMaxFilter);
	Font::SharedPtr LoadFont(const std::string& Name, const std::string& Path, const std::string& ShaderName);
	SoundSharedPtr LoadSound(const std::string& Name, const std::string& Path);
	SoundSharedPtr LoadSequenceSound(const std::string& Name, const std::vector<std::string>& Paths);
	SoundSharedPtr LoadRandomSound(const std::string& Name, const std::vector<std::string>& Paths);

	Shader::SharedPtr GetShader(const std::string& Name);
	Texture::SharedPtr GetTexture(const std::string& Name);
	Font::SharedPtr GetFont(const std::string& Name);
	SoundSharedPtr GetSound(const std::string& Name);

private:
	ShaderMap Shaders;
	TextureMap Textures;
	FontMap Fonts;
	SoundMap Sounds;
};