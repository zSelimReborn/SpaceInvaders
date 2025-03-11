#include "Component.h"

#include "Actor.h"

pk::Component::Component(const ActorWeakPtr& InOwner)
	: Owner(InOwner) { }

void pk::Component::Begin()
{
	
}

void pk::Component::Update(const float Delta)
{
	
}

pk::Component::ActorSharedPtr pk::Component::GetOwner() const
{
	return Owner.lock();
}

