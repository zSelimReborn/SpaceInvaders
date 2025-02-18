#include "Ship.h"

#include <iostream>
#include <GLFW/glfw3.h>

#include "pk/Scene.h"
#include "pk/Window.h"
#include "Projectile.h"
#include "pk/SettingsReader.h"

const float Ship::DEFAULT_SPEED = 450.f;
const float Ship::DEFAULT_COOLDOWN = 1.f;
const float Ship::DEFAULT_PROJECTILE_SPEED = 500.f;
const glm::vec3 Ship::DEFAULT_PROJECTILE_SIZE = glm::vec3(5.f, 10.f, 1.f);

Ship::Ship(const Transform& InTransform)
	: Actor(InTransform), MaxSpeed(DEFAULT_SPEED), Speed(DEFAULT_SPEED),
		ShootCooldown(DEFAULT_COOLDOWN), CurrentCooldown(0.f), bShouldCooldown(false)
{
	
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

void Ship::SetProjectileData(const ProjectileData& InProjectileInfo)
{
	ProjectileInfo = InProjectileInfo;
}

void Ship::LoadConfig()
{
	Actor::LoadConfig();

	ProjectileInfo.Shader = GetShader();
	ProjectileInfo.Direction = glm::vec3(0.f, -1.f, 0.f);
	ProjectileInfo.SpawnOffset = glm::vec3(0.f, -GetSize().y, 0.f);

	Settings::SharedConstPtr ShipSettings = SettingsReader::Load(GetConfigFile());

	if (ShipSettings != nullptr)
	{
		ShipSettings->Get("Speed", DEFAULT_SPEED, MaxSpeed);
		Speed = MaxSpeed;

		ShipSettings->Get("Cooldown", DEFAULT_COOLDOWN, ShootCooldown);
		CurrentCooldown = 0.f;

		ShipSettings->Get("ProjectileSize", ProjectileInfo.Size);
		ShipSettings->Get("ProjectileSpeed", DEFAULT_PROJECTILE_SPEED, ProjectileInfo.Speed);

		glm::vec4 SettingColor(Colors::White);
		ShipSettings->Get("Color", SettingColor);
		SetColor(SettingColor);
	}
	else
	{
		std::cout << "Unable to load player config file. Restoring defaults.\n";
		MaxSpeed = DEFAULT_SPEED;
		Speed = DEFAULT_SPEED;
		ShootCooldown = DEFAULT_COOLDOWN;
		ProjectileInfo.Size = DEFAULT_PROJECTILE_SIZE;
		ProjectileInfo.Speed = DEFAULT_PROJECTILE_SPEED;
	}
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
	const Scene::SharedPtr CurrentScene = GetScene();

	glm::vec3 SpawnLocation(GetLocation());
	SpawnLocation += ProjectileInfo.SpawnOffset;

	glm::vec3 Size(ProjectileInfo.Size);

	glm::vec3 Direction(ProjectileInfo.Direction);
	glm::vec3 Velocity = Direction * ProjectileInfo.Speed;

	Projectile::SharedPtr NewProjectile = std::make_shared<Projectile>(SpawnLocation, Size);
	NewProjectile->SetVelocity(Velocity);
	NewProjectile->SetShader(ProjectileInfo.Shader);
	NewProjectile->SetInitialLifeSpan(ProjectileInfo.InitialLifeSpan);

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
	std::cout << "Hit actor!\n";
}

