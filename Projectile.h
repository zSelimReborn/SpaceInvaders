#pragma once

#include "pk/Actor.h"
#include "pk/IDamageable.h"
#include "Types.h"

#include <functional>
#include <vector>


class TeamComponent;

class Projectile : public Actor, public IDamageable
{
public:
	typedef std::function<void(const Actor::SharedPtr& HitActor, const CollisionResult& Result)> OnHitDelegate;
	typedef std::shared_ptr<Projectile> SharedPtr;
	typedef std::shared_ptr<TeamComponent> TeamComponentPtr;

	Projectile();
	Projectile(const Transform& InTransform);
	Projectile(const glm::vec3& InLocation, const glm::vec3& InSize);

	void SetTeam(Team InTeam) const;
	Team GetTeam() const;

	bool TakeDamage(float InDamage) override;

	void OnHitActor(OnHitDelegate InDelegate);

	void Update(const float Delta) override;

	void Destroy() override;

	~Projectile() override;

private:
	void CheckCollisions();
	void OnHit(const Actor::SharedPtr& HitActor, const CollisionResult& Result);
	void NotifyHit(const Actor::SharedPtr& HitActor, const CollisionResult& Result) const;

	TeamComponentPtr TeamPtr;
	std::vector<OnHitDelegate> OnHitFunctions;
};
