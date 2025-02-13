#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "Window.h"

class Actor;

class Scene
{
public:
	typedef std::shared_ptr<Scene> SharedPtr;
	typedef std::weak_ptr<Scene> WeakPtr;

	typedef std::shared_ptr<Actor> ActorSharedPtr;
	typedef std::vector<ActorSharedPtr>::iterator ActorIterator;

	Scene(const Window::WeakPtr& InWindow);

	virtual void Begin();
	virtual void Frame();
	bool ShouldClose() const;

	int GetScreenWidth() const;
	int GetScreenHeight() const;
	glm::vec2 GetScreenCenter() const;

	glm::mat4 GetProjection() const;

	float GetDelta() const;
	float GetCurrentTime() const;

	Window::SharedPtr GetWindow() const;

	void Add(const ActorSharedPtr& InActor);

	virtual ~Scene();

protected:
	void UpdateDelta();
	void Destroyer();

	virtual void Input(const float Delta);
	virtual void Update(const float Delta);
	virtual void Render(const float Delta);

	Window::WeakPtr WindowPtr;
	std::vector<ActorSharedPtr> Actors;

	glm::mat4 Projection;

	float CurrentTime;
	float OldTime;
	float Delta;

	WeakPtr Self;
};

