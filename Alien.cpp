#include "Alien.h"

#include "pk/SettingsReader.h"

Alien::Alien(AlienType InType)
	: Score(0), Type(InType)
{
}

Alien::Alien(const Transform& InTransform, AlienType InAlienType)
	: Actor(InTransform), Score(0), Type(InAlienType)
{
}

Alien::Alien(const glm::vec3& InLocation, const glm::vec3& InSize, AlienType InAlienType)
	: Alien(Transform(InLocation, InSize), InAlienType)
{
}

void Alien::SetProjectilePool(const std::shared_ptr<ProjectilePool>& InProjectilePool)
{
	ProjectilePoolPtr = InProjectilePool;
}

int Alien::GetScore() const
{
	return Score;
}

AlienType Alien::GetType() const
{
	return Type;
}

void Alien::LoadConfig()
{
	Actor::LoadConfig();

	Settings::SharedConstPtr AlienSettings = SettingsReader::Load(GetConfigFile());
	if (AlienSettings == nullptr)
	{
		return;
	}

	AlienSettings->Get("Score", 0, Score);
	glm::vec4 SettingColor(Colors::White);
	AlienSettings->Get("Color", SettingColor);
	SetColor(SettingColor);
}
