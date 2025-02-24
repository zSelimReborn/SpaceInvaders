#include "Ship.h"

#include <iostream>
#include <GLFW/glfw3.h>

#include "pk/Scene.h"
#include "pk/Window.h"
#include "pk/SettingsReader.h"
#include "Projectile.h"
#include "ProjectilePool.h"
#include "TeamComponent.h"

const float Ship::DEFAULT_SPEED = 450.f;
const float Ship::DEFAULT_COOLDOWN = 1.f;
const int Ship::DEFAULT_LIFE_POINTS = 3;

Ship::Ship(const Transform& InTransform)
	: Actor(InTransform), MaxSpeed(DEFAULT_SPEED), Speed(DEFAULT_SPEED),
		ShootCooldown(DEFAULT_COOLDOWN), CurrentCooldown(0.f), bShouldCooldown(false),
		MaxLifePoints(DEFAULT_LIFE_POINTS), LifePoints(DEFAULT_LIFE_POINTS)
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

void Ship::SetProjectilePool(const ProjectilePoolPtr& InProjectilePool)
{
	CurrentProjectilePool = InProjectilePool;
}

void Ship::LoadConfig()
{
	Actor::LoadConfig();

	Settings::SharedConstPtr ShipSettings = SettingsReader::Load(GetConfigFile());

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
	SetMaxLifePoints(InLifePoints);
	SetLifePoints(InLifePoints);
	SetColor(SettingColor);
	CurrentCooldown = 0.f;
}

float Ship::GetSpeed() const
{
	return Speed;
}

void Ship::Input(const InputHandler& Handler, const float Delta)
{
	glm::vec3 Direction(0.f, 0.f, 0.f);
	if (Handler.IsPressed(GLFW_KEY_LEFT))
	{
		Direction.x = -1.f;
	}
	else if (Handler.IsPressed(GLFW_KEY_RIGHT))
	{
		Direction.x = 1.f;
	}

	if (Handler.IsPressed(GLFW_KEY_SPACE) && CurrentCooldown <= 0.f)
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
	NotifyOnTakeDamage();
	return false;
}

void Ship::AddOnTakeDamageObserver(const OnTakeDamageCallback& Callback)
{
	OnTakeDamageList.push_back(Callback);
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
	if (CurrentProjectilePool == nullptr)
	{
		std::cout << "[Ship] - Unable to shoot, no pool obtained.\n";
		return;
	}

	const Scene::SharedPtr CurrentScene = GetScene();

	glm::vec3 SpawnLocation(GetLocation());
	SpawnLocation.y -= (GetSize().y + 5.f);

	Projectile::SharedPtr NewProjectile = CurrentProjectilePool->Create(SpawnLocation, TeamPtr->GetTeam(), GetShader());
	Projectile::OnHitDelegate Callback = [this](const Actor::SharedPtr& HitActor) { OnProjectileHit(HitActor); };
	NewProjectile->OnHitActor(Callback);

	CurrentScene->Add(NewProjectile);
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
	// TODO Collect score
}

void Ship::NotifyOnTakeDamage() const
{
	for (const OnTakeDamageCallback& Callback : OnTakeDamageList)
	{
		Callback();
	}
}

