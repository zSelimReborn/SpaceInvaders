#pragma once

#include <memory>

namespace pk
{
	class Actor;

	class Component
	{
	public:
		typedef std::shared_ptr<Component> SharedPtr;
		typedef std::shared_ptr<Actor> ActorSharedPtr;
		typedef std::weak_ptr<Actor> ActorWeakPtr;

		Component(const ActorWeakPtr& InOwner);

		virtual void Begin();
		virtual void Update(const float Delta);

		ActorSharedPtr GetOwner() const;

		virtual ~Component() = default;

	private:
		ActorWeakPtr Owner;
	};
}