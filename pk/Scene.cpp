#include "Scene.h"

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
	: WindowPtr(InWindow)
{
	Projection = glm::ortho(0.f, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()), 0.f, -1.0f, 1.0f);
}

void Scene::Begin()
{
	OldTime = static_cast<float>(glfwGetTime());
	for (const Actor::SharedPtr& Actor : Actors)
	{
		Actor->Begin();
	}
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
	for (const Actor::SharedPtr& Actor : Actors)
	{
		Actor->Update(Delta);
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

	for (const Actor::SharedPtr& Actor : Actors)
	{
		Actor->Input(*GetWindow(), Delta);
	}
}

void Scene::Render(const float Delta)
{
	for (const Actor::SharedPtr& Actor : Actors)
	{
		Actor->Render();
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
	if (InActor != nullptr)
	{
		InActor->SetScene(weak_from_this());
		PendingActors.push_back(InActor);
	}
}

void Scene::Destroyer()
{
	Actors.erase(
		std::remove_if(Actors.begin(), Actors.end(), [](const Actor::SharedPtr& Actor) { return Actor == nullptr || Actor->IsDestroyed(); }), 
		Actors.end()
	);
}

void Scene::AddPendingActors()
{
	if (PendingActors.empty())
	{
		return;
	}

	Actors.insert(Actors.end(), PendingActors.begin(), PendingActors.end());
	PendingActors.clear();
}

Scene::~Scene() = default;

void Scene::UpdateDelta()
{
	CurrentTime = static_cast<float>(glfwGetTime());
	Delta = CurrentTime - OldTime;
	OldTime = CurrentTime;
}
