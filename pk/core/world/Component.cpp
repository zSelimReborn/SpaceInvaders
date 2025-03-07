#include "Component.h"

#include "Actor.h"

Component::Component(const ActorWeakPtr& InOwner)
	: Owner(InOwner) { }

void Component::Begin()
{
	
}

void Component::Update(const float Delta)
{
	
}

Component::ActorSharedPtr Component::GetOwner() const
{
	return Owner.lock();
}

