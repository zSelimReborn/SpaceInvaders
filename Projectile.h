#pragma once

#include "pk/Actor.h"
#include "pk/IDamageable.h"
#include "ITeamDefiner.h"

#include <functional>
#include <vector>

class Projectile : public Actor, public ITeamDefiner, public IDamageable
{
public:
	typedef std::function<void(const Actor::SharedPtr& HitActor)> OnHitDelegate;
	typedef std::shared_ptr<Projectile> SharedPtr;

	Projectile();
	Projectile(const Transform& InTransform);
	Projectile(const glm::vec3& InLocation, const glm::vec3& InSize);

	void SetTeam(Team InTeam) override;
	Team GetTeam() const override;

	bool TakeDamage(float InDamage) override;

	void OnHitActor(OnHitDelegate InDelegate);

	void Update(const float Delta) override;

	void Destroy() override;

	~Projectile() override;

private:
	void CheckCollisions();
	void OnHit(const Actor::SharedPtr& HitActor);
	void NotifyHit(const Actor::SharedPtr& HitActor) const;

	Team CurrentTeam;
	std::vector<OnHitDelegate> OnHitFunctions;
};
