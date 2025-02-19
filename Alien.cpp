#include "Alien.h"

#include "pk/SettingsReader.h"

Alien::Alien(const Transform& InTransform)
	: Actor(InTransform), Score(0)
{
}

Alien::Alien(const glm::vec3& InLocation, const glm::vec3& InSize)
	: Alien(Transform(InLocation, InSize))
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
