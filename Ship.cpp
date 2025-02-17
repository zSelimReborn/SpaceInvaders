#include "Ship.h"

#include <iostream>
#include <GLFW/glfw3.h>

#include "pk/Scene.h"
#include "pk/Window.h"
#include "Projectile.h"

Ship::Ship(const Transform& InTransform, float InMaxSpeed)
	: Actor(InTransform), MaxSpeed(InMaxSpeed), Speed(InMaxSpeed),
		ShootCooldown(0.f), CurrentCooldown(0.f), bShouldCooldown(false)
{
	
}

Ship::Ship(const glm::vec3& InLocation, const glm::vec3& InSize, float InMaxSpeed)
	: Ship(Transform(InLocation, InSize), InMaxSpeed)
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

