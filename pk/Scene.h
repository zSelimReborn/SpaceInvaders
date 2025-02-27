#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <memory>

#include "Window.h"
#include "InputHandler.h"

class Actor;
class Widget;

class Scene : public std::enable_shared_from_this<Scene>
{
public:
	typedef std::shared_ptr<Scene> SharedPtr;
	typedef std::weak_ptr<Scene> WeakPtr;

	typedef std::shared_ptr<Actor> ActorSharedPtr;
	typedef std::shared_ptr<Widget> WidgetSharedPtr;
	typedef std::vector<ActorSharedPtr> ActorList;
	typedef std::vector<WidgetSharedPtr> WidgetList;
	typedef std::vector<ActorSharedPtr>::iterator ActorIterator;
	typedef std::map<int, ActorSharedPtr> ActorMap;
	typedef std::pair<int, ActorSharedPtr> ActorMapPair;
	typedef std::map<int, WidgetSharedPtr> WidgetMap;
	typedef std::pair<int, WidgetSharedPtr> WidgetMapPair;

	Scene(const Window::WeakPtr& InWindow);

	int GetNextActorId() const;
	int GetNextWidgetId() const;

	virtual void Begin();
	virtual void Frame();
	bool ShouldClose() const;

	int GetScreenWidth() const;
	int GetScreenHeight() const;
	glm::vec2 GetScreenCenter() const;

	glm::mat4 GetProjection() const;

	float GetDelta() const;
	float GetCurrentTime() const;
	float GetFps() const;

	Window::SharedPtr GetWindow() const;

	void Add(const ActorSharedPtr& InActor);
	std::vector<ActorSharedPtr> GetCollisionActors() const;

	void Add(const WidgetSharedPtr& InWidget);

	virtual ~Scene();

protected:
	void ClearWindow() const;
	void Clean();

	void UpdateDelta();
	void RenderActors() const;
	void RenderWidgets() const;
	void Destroyer();
	void AddPendingActors();
	void UpdateCollisionActorsVector();
	void UpdateActiveWidgets();

	virtual void Input(const float Delta);
	virtual void Update(const float Delta);
	virtual void Render(const float Delta);

	Window::WeakPtr WindowPtr;

	ActorMap Actors;
	ActorMap CollisionActors;
	ActorList PendingActors;
	ActorList CollisionActorsVector;

	WidgetMap ActiveWidgets;
	WidgetList InactiveWidgets;

	glm::mat4 Projection;

	float CurrentTime;
	float OldTime;
	float Delta;
	int Fps;

	int NextActorId;
	int NextWidgetId;

	InputHandler IHandler;
};

