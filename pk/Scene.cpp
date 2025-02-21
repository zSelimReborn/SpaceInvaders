#include "Scene.h"

#include <algorithm>

#include "Window.h"
#include "Common.h"
#include "Font.h"
#include "SoundEngine.h"
#include "Actor.h"

#include <iostream>
#include <sstream>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

Scene::Scene(const Window::WeakPtr& InWindow)
	: WindowPtr(InWindow), NextId(0)
{
	Projection = glm::ortho(0.f, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()), 0.f, -1.0f, 1.0f);
}

int Scene::GetNextId() const
{
	return NextId;
}

void Scene::Begin()
{
	OldTime = static_cast<float>(glfwGetTime());
	AddPendingActors();
}

void Scene::Frame()
{
	UpdateDelta();

	GetWindow()->ClearColor(Colors::LightBlack);
	GetWindow()->ClearFlags(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Input(Delta);
	SoundEngine::Get().Update(Delta);
	Update(Delta);
	Render(Delta);

	AddPendingActors();
	Destroyer();
	GetWindow()->CloseFrame();
}

bool Scene::ShouldClose() const
{
	return GetWindow()->ShouldClose();
}

void Scene::Update(const float Delta)
{
	UpdateCollisionActorsVector();

	for (const ActorMapPair ActorPair : Actors)
	{
		ActorPair.second->Update(Delta);
	}
}

void Scene::Input(const float Delta)
{
	if (WindowPtr.expired())
	{
		return;
	}

	Window::SharedPtr Window = GetWindow();
	if (Window->IsPressed(GLFW_KEY_ESCAPE))
	{
		Window->ShouldClose(true);
	}

	for (const ActorMapPair ActorPair : Actors)
	{
		ActorPair.second->Input(*GetWindow(), Delta);
	}
}

void Scene::Render(const float Delta)
{
	for (const ActorMapPair ActorPair : Actors)
	{
		ActorPair.second->Render();
	}
}

int Scene::GetScreenWidth() const
{
	return GetWindow()->GetWidth();
}

int Scene::GetScreenHeight() const
{
	return GetWindow()->GetHeight();
}

glm::vec2 Scene::GetScreenCenter() const
{
	return GetWindow()->GetScreenCenter();
}

glm::mat4 Scene::GetProjection() const
{
	return Projection;
}

float Scene::GetDelta() const
{
	return Delta;
}

float Scene::GetCurrentTime() const
{
	return CurrentTime;
}

Window::SharedPtr Scene::GetWindow() const
{
	return WindowPtr.lock();
}

void Scene::Add(const Actor::SharedPtr& InActor)
{
	if (Actors.count(InActor->GetId()) > 0)
	{
		return;
	}

	if (InActor != nullptr)
	{
		InActor->Id = NextId++;
		InActor->SetScene(weak_from_this());
		InActor->Begin();
		PendingActors.push_back(InActor);
	}
}

std::vector<Scene::ActorSharedPtr> Scene::GetCollisionActors() const
{
	return CollisionActorsVector;
}

void Scene::Destroyer()
{
	std::vector<int> RemovingIds;
	for (const ActorMapPair ActorPair : Actors)
	{
		const ActorSharedPtr Actor = ActorPair.second;
		if (Actor == nullptr || Actor->IsDestroyed())
		{
			RemovingIds.push_back(Actor->GetId());
			Actor->Id = -1;
		}
	}

	for (int Id : RemovingIds)
	{
		Actors.erase(Id);
		CollisionActors.erase(Id);
	}
}

void Scene::AddPendingActors()
{
	if (PendingActors.empty())
	{
		return;
	}

	for (const ActorSharedPtr& Actor : PendingActors)
	{
		Actors.insert(ActorMapPair(Actor->GetId(), Actor));
		if (Actor->HasCollision())
		{
			CollisionActors.insert(ActorMapPair(Actor->GetId(), Actor));
		}
	}

	PendingActors.clear();
}

void Scene::UpdateCollisionActorsVector()
{
	CollisionActorsVector.clear();
	CollisionActorsVector.reserve(CollisionActors.size());

	for (const ActorMapPair ActorPair : CollisionActors)
	{
		CollisionActorsVector.push_back(ActorPair.second);
	}
}

Scene::~Scene() = default;

void Scene::UpdateDelta()
{
	CurrentTime = static_cast<float>(glfwGetTime());
	Delta = CurrentTime - OldTime;
	OldTime = CurrentTime;
}
