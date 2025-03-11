#pragma once

#include "../Types.h"
#include "../../pk/core/world/Component.h"

using namespace pk;

class TeamComponent : public Component
{
public:
	typedef std::shared_ptr<TeamComponent> SharedPtr;

	TeamComponent(const ActorWeakPtr& InOwner);

	void SetTeam(Team InTeam);
	Team GetTeam() const;

private:
	Team CurrentTeam;
};