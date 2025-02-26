#include "Brick.h"

Brick::Brick()
{
	HasCollision(true);
	TeamPtr = std::make_shared<TeamComponent>(weak_from_this());
	TeamPtr->SetTeam(Team::Brick);
	AddComponent(TeamPtr);
}

bool Brick::TakeDamage(float InDamage)
{
	Destroy();
	return true;
}
