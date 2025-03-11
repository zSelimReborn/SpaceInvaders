#pragma once

#include "../../pk/core/world/Actor.h"
#include "../../pk/core/interfaces/IDamageable.h"
#include "../Types.h"

#include <functional>
#include <vector>

class TeamComponent;

using namespace pk;

class Projectile : public Actor, public IDamageable
{
public:
	typedef std::function<void(const Actor::SharedPtr& HitActor, const CollisionResult& Result)> OnHitDelegate;
	typedef std::function<void()> OnDestroyDelegate;
	typedef std::shared_ptr<Projectile> SharedPtr;
	typedef std::shared_ptr<TeamComponent> TeamComponentPtr;

	Projectile();
	Projectile(const Transform& InTransform);
	Projectile(const glm::vec3& InLocation, const glm::vec3& InSize);

	void SetTeam(Team InTeam) const;
	Team GetTeam() const;

	bool TakeDamage(float InDamage) override;

	void AddOnHitDelegate(const OnHitDelegate& InDelegate);
	void AddOnDestroyDelegate(const OnDestroyDelegate& InDelegate);

	void OnActorHit(const Actor::SharedPtr& HitActor, const CollisionResult& Result) override;

	void Update(const float Delta) override;

	void Destroy() override;

	~Projectile() override;

private:
	void OnHit(const Actor::SharedPtr& HitActor, const CollisionResult& Result);
	void NotifyHit(const Actor::SharedPtr& HitActor, const CollisionResult& Result) const;
	void NotifyDestroy() const;

	TeamComponentPtr TeamPtr;
	std::vector<OnHitDelegate> OnHitFunctions;
	std::vector<OnDestroyDelegate> OnDestroyFunctions;
};
