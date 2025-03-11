#include "Projectile.h"

#include <iostream>

#include "../components/TeamComponent.h"
#include "../../pk/core/world/Scene.h"

Projectile::Projectile()
{
	HasCollision(true);

	TeamPtr = std::make_shared<TeamComponent>(weak_from_this());
	AddComponent(TeamPtr);
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

void Projectile::SetTeam(Team InTeam) const
{
	TeamPtr->SetTeam(InTeam);
}

Team Projectile::GetTeam() const
{
	return TeamPtr->GetTeam();
}

bool Projectile::TakeDamage(float InDamage)
{
	Destroy();
	return true;
}

void Projectile::AddOnHitDelegate(const OnHitDelegate& InDelegate)
{
	OnHitFunctions.push_back(InDelegate);
}

void Projectile::AddOnDestroyDelegate(const OnDestroyDelegate& InDelegate)
{
	OnDestroyFunctions.push_back(InDelegate);
}

void Projectile::OnActorHit(const Actor::SharedPtr& HitActor, const CollisionResult& Result)
{
	Actor::OnActorHit(HitActor, Result);

	OnHit(HitActor, Result);
}

void Projectile::Update(const float Delta)
{
	Actor::Update(Delta);
	if (!IsInViewport())
	{
		Destroy();
	}
}

void Projectile::Destroy()
{
	NotifyDestroy();

	Actor::Destroy();
	OnHitFunctions.clear();
	OnDestroyFunctions.clear();
}

Projectile::~Projectile() = default;

void Projectile::OnHit(const Actor::SharedPtr& HitActor, const CollisionResult& Result)
{
	const TeamComponentPtr HitTeamComponent = HitActor->GetComponent<TeamComponent>();
	// Skip same team
	if (!HitTeamComponent || GetTeam() == HitTeamComponent->GetTeam())
	{
		return;
	}

	std::shared_ptr<IDamageable> DamageableActor = std::dynamic_pointer_cast<IDamageable>(HitActor);
	if (!DamageableActor)
	{
		return;
	}

	NotifyHit(HitActor, Result);
	DamageableActor->TakeDamage(0.f);
	Destroy();
}

void Projectile::NotifyHit(const Actor::SharedPtr& HitActor, const CollisionResult& Result) const
{
	for (const OnHitDelegate& Delegate : OnHitFunctions)
	{
		Delegate(HitActor, Result);
	}
}

void Projectile::NotifyDestroy() const
{
	for (const OnDestroyDelegate& Delegate : OnDestroyFunctions)
	{
		Delegate();
	}
}
