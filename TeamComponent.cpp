#include "TeamComponent.h"

TeamComponent::TeamComponent(const ActorWeakPtr& InOwner)
	: Component(InOwner), CurrentTeam(Team::None)
{
	
}

void TeamComponent::SetTeam(Team InTeam)
{
	CurrentTeam = InTeam;
}

Team TeamComponent::GetTeam() const
{
	return CurrentTeam;
}

