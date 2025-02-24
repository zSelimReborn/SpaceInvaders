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

Ship::Ship(const Transform& InTransform)
	: Actor(InTransform), MaxSpeed(DEFAULT_SPEED), Speed(DEFAULT_SPEED),
		ShootCooldown(DEFAULT_COOLDOWN), CurrentCooldown(0.f), bShouldCooldown(false)
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

float Ship::GetShootCooldown() const
{
	return ShootCooldown;
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

	ShipSettings->Get("Speed", DEFAULT_SPEED, MaxSpeed);
	Speed = MaxSpeed;

	ShipSettings->Get("Cooldown", DEFAULT_COOLDOWN, ShootCooldown);
	CurrentCooldown = 0.f;

	glm::vec4 SettingColor(Colors::White);
	ShipSettings->Get("Color", SettingColor);
	SetColor(SettingColor);
}

float Ship::GetSpeed() const
{
	return Speed;
}

void Ship::Input(const Window& Window, const float Delta)
{
	glm::vec3 Direction(0.f, 0.f, 0.f);
	if (Window.IsPressed(GLFW_KEY_LEFT))
	{
		Direction.x = -1.f;
	}
	else if (Window.IsPressed(GLFW_KEY_RIGHT))
	{
		Direction.x = 1.f;
	}

	if (Window.IsPressed(GLFW_KEY_SPACE) && CurrentCooldown <= 0.f)
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
	// TODO Define lives
	return false;
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

