#include "Projectile.h"

#include <iostream>

#include "pk/Scene.h"

Projectile::Projectile(const Transform& InTransform)
	: Actor(InTransform)
{
}

Projectile::Projectile(const glm::vec3& InLocation, const glm::vec3& InSize)
	: Actor(InLocation, InSize)
{
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
}

Projectile::~Projectile()
{
	std::cout << "Projectile destroyed!\n";
}

void Projectile::NotifyHit(const Actor::SharedPtr& HitActor) const
{
	for (const OnHitDelegate& Delegate : OnHitFunctions)
	{
		Delegate(HitActor);
	}
}
