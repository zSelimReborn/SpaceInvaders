#include "Scene.h"

#include "../window/Window.h"
#include "../utils/Common.h"
#include "../asset/Font.h"
#include "../../sound/SoundEngine.h"
#include "../../ui/Widget.h"

#include "Actor.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>


Scene::Scene()
	: CurrentTime(0.f), OldTime(0.f), Delta(0.f), Fps(0.f), NextActorId(0), NextWidgetId(0)
{
	IHandler.HandleKey(GLFW_KEY_ESCAPE, InputType::Press);
}

Scene::Scene(Window::WeakPtr InWindow)
	: Scene()
{
	SetWindow(std::move(InWindow));
}

int Scene::GetNextActorId() const
{
	return NextActorId;
}

int Scene::GetNextWidgetId() const
{
	return NextWidgetId;
}

void Scene::Begin()
{
	const Window::SharedPtr CurrentWindow = GetWindow();
	if (CurrentWindow != nullptr)
	{
		CurrentWindow->SetOnCloseFunction([this]() { Quit(); });
	}

	OldTime = static_cast<float>(glfwGetTime());
	AddPendingActors();
}

void Scene::Frame()
{
	UpdateDelta();

	ClearWindow();

	Input(Delta);
	SoundEngine::Get().Update(Delta);
	Update(Delta);
	Render(Delta);

	Clean();
}

void Scene::Quit()
{
	const Window::SharedPtr CurrentWindow = GetWindow();
	if (CurrentWindow == nullptr)
	{
		return;
	}

	CurrentWindow->ShouldClose(true);
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
	IHandler.Update(*Window, Delta);

	HandleInput(Delta);

	IHandler.Clean();
}

void Scene::HandleInput(const float Delta)
{
	HandleActorsInput(Delta);
	HandleWidgetInput(Delta);
}

void Scene::Render(const float Delta)
{
	RenderActors();
	RenderWidgets();
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

float Scene::GetFps() const
{
	return Fps;
}

void Scene::SetWindow(Window::WeakPtr InWindow)
{
	WindowPtr = std::move(InWindow);
	OnSetWindow();
}

Window::SharedPtr Scene::GetWindow() const
{
	return WindowPtr.lock();
}

void Scene::Add(const Actor::SharedPtr& InActor)
{
	if (InActor == nullptr)
	{
		return;
	}

	if (Actors.count(InActor->GetId()) > 0)
	{
		return;
	}

	InActor->Id = NextActorId++;
	InActor->SetScene(weak_from_this());
	InActor->Begin();
	PendingActors.push_back(InActor);
}

std::vector<Scene::ActorSharedPtr> Scene::GetCollisionActors() const
{
	return CollisionActorsVector;
}

void Scene::Add(const WidgetSharedPtr& InWidget)
{
	if (InWidget == nullptr)
	{
		return;
	}

	if (ActiveWidgets.count(InWidget->GetId()) > 0)
	{
		return;
	}

	InWidget->Id = NextWidgetId++;
	InWidget->SetScene(weak_from_this());
	InWidget->Construct();
	InactiveWidgets.push_back(InWidget);
}

void Scene::Destroyer()
{
	std::vector<int> RemovingIds;
	for (const ActorMapPair ActorPair : Actors)
	{
		const ActorSharedPtr Actor = ActorPair.second;
		if (Actor == nullptr || Actor->IsDestroyed())
		{
			RemovingIds.push_back(ActorPair.first);
			if (Actor != nullptr)
			{
				Actor->Id = -1;
			}
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

void Scene::UpdateActiveWidgets()
{
	for (const WidgetSharedPtr& Widget : InactiveWidgets)
	{
		if (Widget == nullptr)
		{
			continue;
		}

		ActiveWidgets.insert(WidgetMapPair(Widget->GetId(), Widget));
	}

	InactiveWidgets.clear();

	std::vector<int> RemovingIds;
	for (const WidgetMapPair WidgetPair : ActiveWidgets)
	{
		const WidgetSharedPtr Widget = WidgetPair.second;
		if (Widget == nullptr || !Widget->IsActive())
		{
			RemovingIds.push_back(WidgetPair.first);
		}
	}

	for (int& Id : RemovingIds)
	{
		const WidgetSharedPtr Widget = ActiveWidgets[Id];
		if (Widget != nullptr)
		{
			InactiveWidgets.push_back(Widget);
		}

		ActiveWidgets.erase(Id);
	}
}

Scene::~Scene() = default;

void Scene::OnSetWindow()
{
	if (WindowPtr.expired())
	{
		return;
	}

	Projection = glm::ortho(0.f, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()), 0.f, -1.0f, 1.0f);
}

void Scene::ClearWindow() const
{
	GetWindow()->ClearColor(Colors::LightBlack);
	GetWindow()->ClearFlags(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Scene::Clean()
{
	AddPendingActors();
	Destroyer();
	UpdateActiveWidgets();
	GetWindow()->CloseFrame();
}

void Scene::UpdateDelta()
{
	CurrentTime = static_cast<float>(glfwGetTime());
	Delta = CurrentTime - OldTime;
	OldTime = CurrentTime;
	Fps = 1 / Delta;
}

void Scene::RenderActors() const
{
	for (const ActorMapPair ActorPair : Actors)
	{
		ActorPair.second->Render();
	}
}

void Scene::RenderWidgets() const
{
	for (const WidgetMapPair WidgetPair : ActiveWidgets)
	{
		WidgetPair.second->Render();
	}
}

void Scene::HandleActorsInput(const float Delta) const
{
	for (const ActorMapPair ActorPair : Actors)
	{
		ActorPair.second->Input(IHandler, Delta);
	}
}

void Scene::HandleWidgetInput(const float Delta) const
{
	for (const WidgetMapPair WidgetMap : ActiveWidgets)
	{
		WidgetMap.second->Input(IHandler, Delta);
	}
}
