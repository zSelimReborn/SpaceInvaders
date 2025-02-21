#include "Alien.h"

#include <iostream>

#include "Projectile.h"
#include "ProjectilePool.h"
#include "pk/Scene.h"
#include "pk/SettingsReader.h"

Alien::Alien(AlienType InType)
	: Score(0), Type(InType), CurrentTeam(Team::Alien)
{
	HasCollision(true);
}

Alien::Alien(const Transform& InTransform, AlienType InAlienType)
	: Alien(InAlienType)
{
	SetLocation(InTransform.Location);
	SetSize(InTransform.Size);
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

void Alien::SetTeam(Team InTeam)
{
	CurrentTeam = InTeam;
}

Team Alien::GetTeam() const
{
	return CurrentTeam;
}

bool Alien::TakeDamage(float InDamage)
{
	Destroy();
	return true;
}

void Alien::Shoot()
{
	if (ProjectilePoolPtr == nullptr)
	{
		std::cout << "[Alien] - Unable to shoot, no pool obtained.\n";
		return;
	}

	const Scene::SharedPtr CurrentScene = GetScene();

	glm::vec3 SpawnLocation(GetLocation());
	SpawnLocation.y += (GetSize().y + 5.f);

	Projectile::SharedPtr NewProjectile = ProjectilePoolPtr->Create(SpawnLocation, CurrentTeam, GetShader());
	CurrentScene->Add(NewProjectile);
}
