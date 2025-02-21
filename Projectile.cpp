#include "Projectile.h"

#include <iostream>

#include "pk/Scene.h"

Projectile::Projectile()
	: CurrentTeam(Team::Human)
{
	HasCollision(true);
}

Projectile::Projectile(const Transform& InTransform)
	: Projectile()
{
	SetLocation(InTransform.Location);
	SetSize(InTransform.Size);
}

Projectile::Projectile(const glm::vec3& InLocation, const glm::vec3& InSize)
	: Projectile(Transform(InLocation, InSize))
{
}

void Projectile::SetTeam(Team InTeam)
{
	CurrentTeam = InTeam;
}

Team Projectile::GetTeam() const
{
	return CurrentTeam;
}

void Projectile::OnHitActor(OnHitDelegate InDelegate)
{
	OnHitFunctions.push_back(InDelegate);
}

void Projectile::Update(const float Delta)
{
	Actor::Update(Delta);
	if (!IsInViewport())
	{
		Destroy();
	}

	CheckCollisions();
}

void Projectile::Destroy()
{
	Actor::Destroy();
	OnHitFunctions.clear();
}

Projectile::~Projectile()
{
	std::cout << "Projectile destroyed!\n";
}

void Projectile::CheckCollisions()
{
	const Scene::SharedPtr CurrentScene = GetScene();
	if (CurrentScene == nullptr)
	{
		return;
	}

	const std::vector<Actor::SharedPtr> CollisionActors = CurrentScene->GetCollisionActors();
	for (const Actor::SharedPtr& CheckingActor : CollisionActors)
	{
		if (!CheckingActor 
			|| CheckingActor->IsDestroyed() 
			|| CheckingActor->GetId() == GetId()
		) {
			continue;
		}

		if (Collide(*CheckingActor))
		{
			OnHit(CheckingActor);
		}
	}
}

void Projectile::OnHit(const Actor::SharedPtr& HitActor)
{
	// TODO Deal damage
	std::shared_ptr<ITeamDefiner> TeamActor = std::dynamic_pointer_cast<ITeamDefiner>(HitActor);
	// Skip same team
	if (!TeamActor || TeamActor->GetTeam() == GetTeam())
	{
		return;
	}

	std::cout << "Hit Actor\n";
	NotifyHit(HitActor);
	Destroy();
}

void Projectile::NotifyHit(const Actor::SharedPtr& HitActor) const
{
	for (const OnHitDelegate& Delegate : OnHitFunctions)
	{
		Delegate(HitActor);
	}
}
