#pragma once

#include "Types.h"
#include "pk/Component.h"


class TeamComponent : public Component
{
public:
	TeamComponent(const ActorWeakPtr& InOwner);

	void SetTeam(Team InTeam);
	Team GetTeam() const;

private:
	Team CurrentTeam;
};