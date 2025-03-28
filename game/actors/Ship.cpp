#include "Ship.h"

#include <iostream>
#include <GLFW/glfw3.h>

#include "Alien.h"
#include "Projectile.h"
#include "../Assets.h"
#include "../scenes/Game.h"
#include "../pools/ProjectilePool.h"
#include "../components/TeamComponent.h"

#include "../../pk/core/world/Scene.h"
#include "../../pk/core/window/Window.h"
#include "../../pk/core/utils/ClassSettingsReader.h"
#include "../../pk/sound/SoundEngine.h"

const float Ship::DEFAULT_SPEED = 450.f;
const float Ship::DEFAULT_COOLDOWN = 1.f;
const int Ship::DEFAULT_LIFE_POINTS = 3;

Ship::Ship(const Transform& InTransform)
	: Actor(InTransform), bCanShoot(true), MaxSpeed(DEFAULT_SPEED), Speed(DEFAULT_SPEED),
		ShootCooldown(DEFAULT_COOLDOWN), CurrentCooldown(0.f), bShouldCooldown(false),
		MaxLifePoints(DEFAULT_LIFE_POINTS), LifePoints(DEFAULT_LIFE_POINTS),
		ScorePoints(0)
{
	HasCollision(true);

	TeamPtr = std::make_shared<TeamComponent>(weak_from_this());
	TeamPtr->SetTeam(Team::Human);
	AddComponent(TeamPtr);
}

Ship::Ship(const glm::vec3& InLocation, const glm::vec3& InSize)
	: Ship(Transform(InLocation, InSize))
{
	
}

void Ship::SetMaxSpeed(float InSpeed)
{
	MaxSpeed = std::abs(InSpeed);
}

void Ship::SetSpeed(float InSpeed)
{
	Speed = std::min(MaxSpeed, InSpeed);
}

float Ship::GetMaxSpeed() const
{
	return MaxSpeed;
}

void Ship::SetShootCooldown(float InCooldown)
{
	ShootCooldown = std::abs(InCooldown);
}

void Ship::SetMaxLifePoints(int InLifePoints)
{
	MaxLifePoints = std::abs(InLifePoints);
}

float Ship::GetShootCooldown() const
{
	return ShootCooldown;
}

int Ship::GetMaxLifePoints() const
{
	return MaxLifePoints;
}

void Ship::SetLifePoints(int InLifePoints)
{
	LifePoints = std::min(MaxLifePoints, InLifePoints);
}

int Ship::GetLifePoints() const
{
	return LifePoints;
}

int Ship::GetScorePoints() const
{
	return ScorePoints;
}

void Ship::ResetLifePoints()
{
	LifePoints = MaxLifePoints;
}

void Ship::ResetScorePoints()
{
	ScorePoints = 0;
}

void Ship::SetProjectilePool(const ProjectilePoolPtr& InProjectilePool)
{
	CurrentProjectilePool = InProjectilePool;
}

void Ship::LoadConfig()
{
	Actor::LoadConfig();

	ClassSettings::SharedConstPtr ShipSettings = ClassSettingsReader::Load(GetConfigFile());

	if (ShipSettings == nullptr)
	{
		std::cout << "Unable to load player config file. Restoring defaults.\n";
		return;
	}

	float InMaxSpeed, InShootCooldown;
	int InLifePoints;
	glm::vec4 SettingColor(Colors::White);
	ShipSettings->Get("Speed", DEFAULT_SPEED, InMaxSpeed);
	ShipSettings->Get("Cooldown", DEFAULT_COOLDOWN, InShootCooldown);
	ShipSettings->Get("LifePoints", DEFAULT_LIFE_POINTS, InLifePoints);
	ShipSettings->Get("Color", SettingColor);

	SetMaxSpeed(InMaxSpeed);
	SetSpeed(InMaxSpeed);
	SetShootCooldown(InShootCooldown);
	SetMaxLifePoints(InLifePoints);
	SetLifePoints(InLifePoints);
	SetColor(SettingColor);

	CurrentCooldown = 0.f;
}

void Ship::Begin()
{
	Actor::Begin();

	GamePtr = std::dynamic_pointer_cast<Game>(GetScene());
}

float Ship::GetSpeed() const
{
	return Speed;
}

void Ship::Input(const InputHandler& Handler, const float Delta)
{
	glm::vec3 Direction(0.f, 0.f, 0.f);
	if (Handler.IsPressed(GLFW_KEY_LEFT) || Handler.IsPadPressed(GLFW_GAMEPAD_BUTTON_DPAD_LEFT))
	{
		Direction.x = -1.f;
	}
	else if (Handler.IsPressed(GLFW_KEY_RIGHT) || Handler.IsPadPressed(GLFW_GAMEPAD_BUTTON_DPAD_RIGHT))
	{
		Direction.x = 1.f;
	}

	if ((Handler.IsPressed(GLFW_KEY_SPACE) 
		|| Handler.IsPadPressed(GLFW_GAMEPAD_BUTTON_SQUARE)) 
		&& CurrentCooldown <= 0.f
	)
	{
		bShouldCooldown = true;
		Shoot();
	}

	SetVelocity(Direction * Speed);
}

void Ship::Update(const float Delta)
{
	Actor::Update(Delta);

	UpdateCooldown(Delta);
	ConstraintInViewport(Delta);
}

bool Ship::TakeDamage(float InDamage)
{
	LifePoints = std::max(0, LifePoints - 1);

	PlayAudio(Assets::Sounds::PlayerExplosionName, Assets::Sounds::PlayerExplosion, 1.f);
	NotifyOnTakeDamage();
	return false;
}

void Ship::AddOnTakeDamageObserver(const OnTakeDamageDelegate& Callback)
{
	OnTakeDamageFunctions.push_back(Callback);
}

void Ship::ConstraintInViewport(const float Delta)
{
	const glm::vec3 Location(GetLocation());
	const BoundingBox Box(GetBoundingBox());
	const glm::vec2 ScaleOffset(Box.ScaleOffset);

	const Scene::SharedPtr CurrentScene = GetScene();
	if (CurrentScene == nullptr)
	{
		std::cout << "Empty scene\n";
		return;
	}

	const Window::SharedPtr CurrentWindow = CurrentScene->GetWindow();
	if (CurrentWindow == nullptr)
	{
		std::cout << "Empty window\n";
		return;
	}

	glm::vec3 NewLocation(Location);
	const float WindowWidth = static_cast<float>(CurrentWindow->GetWidth());
	if (Location.x - ScaleOffset.x <= 0.f)
	{
		NewLocation.x = ScaleOffset.x;
	}
	else if (Location.x + ScaleOffset.x >= WindowWidth)
	{
		NewLocation.x = WindowWidth - ScaleOffset.x;
	}

	SetLocation(NewLocation);
}

void Ship::Shoot()
{
	if (!bCanShoot)
	{
		return;
	}

	if (CurrentProjectilePool == nullptr)
	{
		std::cout << "[Ship] - Unable to shoot, no pool obtained.\n";
		return;
	}

	const Scene::SharedPtr CurrentScene = GetScene();

	glm::vec3 SpawnLocation(GetLocation());
	SpawnLocation.y -= (GetSize().y / 2);

	Projectile::SharedPtr NewProjectile = CurrentProjectilePool->Create(SpawnLocation, TeamPtr->GetTeam());
	NewProjectile->AddOnHitDelegate([this](const Actor::SharedPtr& HitActor, const CollisionResult& Result)
		{
			OnProjectileHit(HitActor);
		}
	);
	NewProjectile->AddOnDestroyDelegate([this]()
		{
			OnProjectileDestroy();
		}
	);

	CurrentScene->Add(NewProjectile);

	PlayAudio(Assets::Sounds::ShootName, Assets::Sounds::OldShoot, 1.f);
	bCanShoot = false;
}

void Ship::UpdateCooldown(const float Delta)
{
	if (!bShouldCooldown)
	{
		return;
	}

	CurrentCooldown += Delta;
	if (CurrentCooldown >= ShootCooldown)
	{
		bShouldCooldown = false;
		CurrentCooldown = 0.f;
	}
}

void Ship::OnProjectileHit(const Actor::SharedPtr& HitActor)
{
	Alien::SharedPtr HitAlien = std::dynamic_pointer_cast<Alien>(HitActor);
	if (HitAlien == nullptr)
	{
		return;
	}

	ScorePoints += HitAlien->GetScore();
}

void Ship::OnProjectileDestroy()
{
	bCanShoot = true;
}

void Ship::NotifyOnTakeDamage() const
{
	for (const OnTakeDamageDelegate& Callback : OnTakeDamageFunctions)
	{
		Callback();
	}
}

void Ship::PlayAudio(const std::string& Name, const std::string& FilePath, float Volume) const
{
	const GameSharedPtr CurrentGame = GetGame();
	if (CurrentGame == nullptr)
	{
		SoundEngine::Get().Play(FilePath, Volume);
	}
	else
	{
		CurrentGame->PlayAudio(Name, Volume);
	}
}

Ship::GameSharedPtr Ship::GetGame() const
{
	return GamePtr.lock();
}

