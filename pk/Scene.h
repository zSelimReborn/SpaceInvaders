#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <memory>

#include "Window.h"
#include "InputHandler.h"

class Actor;

class Scene : public std::enable_shared_from_this<Scene>
{
public:
	typedef std::shared_ptr<Scene> SharedPtr;
	typedef std::weak_ptr<Scene> WeakPtr;

	typedef std::shared_ptr<Actor> ActorSharedPtr;
	typedef std::vector<ActorSharedPtr>::iterator ActorIterator;
	typedef std::map<int, ActorSharedPtr> ActorMap;
	typedef std::pair<int, ActorSharedPtr> ActorMapPair;

	Scene(const Window::WeakPtr& InWindow);

	int GetNextId() const;

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
	std::vector<ActorSharedPtr> GetCollisionActors() const;

	virtual ~Scene();

protected:
	void UpdateDelta();
	void Destroyer();
	void AddPendingActors();
	void UpdateCollisionActorsVector();

	virtual void Input(const float Delta);
	virtual void Update(const float Delta);
	virtual void Render(const float Delta);

	Window::WeakPtr WindowPtr;
	ActorMap Actors;
	ActorMap CollisionActors;
	std::vector<ActorSharedPtr> PendingActors;
	std::vector<ActorSharedPtr> CollisionActorsVector;

	glm::mat4 Projection;

	float CurrentTime;
	float OldTime;
	float Delta;

	int NextId;

	InputHandler IHandler;
};

