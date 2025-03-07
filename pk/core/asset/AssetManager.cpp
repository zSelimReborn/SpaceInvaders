#include "AssetManager.h"

#include "../../sound/RandomSound.h"
#include "../../sound/SequenceSound.h"
#include "../../sound/SimpleSound.h"
#include "../../sound/SoundEngine.h"

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

AssetManager::SoundSharedPtr AssetManager::LoadSound(const std::string& Name, const std::string& Path)
{
	SoundSharedPtr FoundSound = GetSound(Name);
	if (FoundSound != nullptr)
	{
		return FoundSound;
	}

	std::shared_ptr<SimpleSound> NewSound = std::make_shared<SimpleSound>(Path);
	SoundEngine::Get().Load(Path);
	Sounds.insert(SoundPair(Name, NewSound));

	return NewSound;
}

AssetManager::SoundSharedPtr AssetManager::LoadSequenceSound(const std::string& Name,
	const std::vector<std::string>& Paths)
{
	SoundSharedPtr FoundSound = GetSound(Name);
	if (FoundSound != nullptr)
	{
		return FoundSound;
	}

	std::shared_ptr<SequenceSound> NewSound = std::make_shared<SequenceSound>(Paths);
	for (const std::string& Path : Paths)
	{
		SoundEngine::Get().Load(Path);
	}

	Sounds.insert(SoundPair(Name, NewSound));
	return NewSound;
}

AssetManager::SoundSharedPtr AssetManager::LoadRandomSound(const std::string& Name,
	const std::vector<std::string>& Paths)
{
	SoundSharedPtr FoundSound = GetSound(Name);
	if (FoundSound != nullptr)
	{
		return FoundSound;
	}

	std::shared_ptr<RandomSound> NewSound = std::make_shared<RandomSound>(Paths);
	for (const std::string& Path : Paths)
	{
		SoundEngine::Get().Load(Path);
	}

	Sounds.insert(SoundPair(Name, NewSound));
	return NewSound;
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

AssetManager::SoundSharedPtr AssetManager::GetSound(const std::string& Name)
{
	if (Sounds.count(Name) <= 0)
	{
		return nullptr;
	}

	return Sounds[Name];
}

Shader::SharedPtr AssetManager::GetShader(const std::string& Name)
{
	if (Shaders.count(Name) <= 0)
	{
		return nullptr;
	}

	return Shaders[Name];
}
