#pragma once
#include "pk/Actor.h"

#include <functional>
#include <vector>

class Projectile : public Actor
{
public:
	typedef std::function<void(const Actor::SharedPtr& HitActor)> OnHitDelegate;
	typedef std::shared_ptr<Projectile> SharedPtr;

	Projectile(const Transform& InTransform);
	Projectile(const glm::vec3& InLocation, const glm::vec3& InSize);

	void OnHitActor(OnHitDelegate InDelegate);

	void Update(const float Delta) override;

	virtual ~Projectile();

private:
	void NotifyHit(const Actor::SharedPtr& HitActor) const;

	std::vector<OnHitDelegate> OnHitFunctions;
};
