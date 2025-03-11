#include "Scene.h"

#include "../window/Window.h"
#include "../utils/Common.h"
#include "../asset/Font.h"
#include "../../sound/SoundEngine.h"
#include "../../ui/Widget.h"

#include "Actor.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "../collisions/QuadPool.h"
#include "../collisions/QuadTree.h"

using namespace pk;

pk::Scene::Scene()
	: CollisionRoot(nullptr), CurrentTime(0.f), OldTime(0.f), Delta(0.f), Fps(0.f), NextActorId(0), NextWidgetId(0)
{
	IHandler.HandleKey(GLFW_KEY_ESCAPE, InputType::Press);
}

pk::Scene::Scene(Window::WeakPtr InWindow)
	: pk::Scene()
{
	SetWindow(std::move(InWindow));
}

int pk::Scene::GetNextActorId() const
{
	return NextActorId;
}

int pk::Scene::GetNextWidgetId() const
{
	return NextWidgetId;
}

void pk::Scene::Begin()
{
	const Window::SharedPtr CurrentWindow = GetWindow();
	if (CurrentWindow != nullptr)
	{
		CurrentWindow->SetOnCloseFunction([this]() { Quit(); });
	}

	OldTime = static_cast<float>(glfwGetTime());
	AddPendingActors();
}

void pk::Scene::Frame()
{
	UpdateDelta();

	ClearWindow();

	Input(Delta);
	SoundEngine::Get().Update(Delta);
	Update(Delta);
	Render(Delta);

	Clean();
}

void pk::Scene::Quit()
{
	const Window::SharedPtr CurrentWindow = GetWindow();
	if (CurrentWindow == nullptr)
	{
		return;
	}

	CurrentWindow->ShouldClose(true);
}

bool pk::Scene::ShouldClose() const
{
	return GetWindow()->ShouldClose();
}

void pk::Scene::Update(const float Delta)
{
	BuildCollisionTree();

	for (const ActorMapPair ActorPair : Actors)
	{
		ActorPair.second->Update(Delta);
	}

	CheckCollisions(Delta);
}

void pk::Scene::Input(const float Delta)
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

void pk::Scene::HandleInput(const float Delta)
{
	HandleActorsInput(Delta);
	HandleWidgetInput(Delta);
}

void pk::Scene::Render(const float Delta)
{
	RenderActors();
	RenderWidgets();
}

int pk::Scene::GetScreenWidth() const
{
	return GetWindow()->GetWidth();
}

int pk::Scene::GetScreenHeight() const
{
	return GetWindow()->GetHeight();
}

glm::vec2 pk::Scene::GetScreenCenter() const
{
	return GetWindow()->GetScreenCenter();
}

glm::mat4 pk::Scene::GetProjection() const
{
	return Projection;
}

float pk::Scene::GetDelta() const
{
	return Delta;
}

float pk::Scene::GetCurrentTime() const
{
	return CurrentTime;
}

float pk::Scene::GetFps() const
{
	return Fps;
}

void pk::Scene::BuildCollisionTree()
{
	QuadPool::Get().Reset();
	CollisionRoot = QuadPool::Get().GetQuadTree();
	CollisionRoot->Reset(glm::vec3(0.f), static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()), 0);
	for (const ActorMapPair ActorPair : CollisionActors)
	{
		int Id = ActorPair.first;
		const ActorSharedPtr Actor = ActorPair.second;
		CollisionRoot->Insert(Id, Actor->GetLocation());
	}
}

void pk::Scene::CheckCollisions(float Delta)
{
	for (const ActorMapPair ActorPair : CollisionActors)
	{
		int Id = ActorPair.first;
		const ActorSharedPtr Actor = ActorPair.second;
		QuadTree* FoundQuad = CollisionRoot->Search(Actor->GetLocation());
		if (FoundQuad == nullptr)
		{
			continue;
		}

		for (int QuadActorId : FoundQuad->GetEntities())
		{
			const ActorSharedPtr QuadActor = CollisionActors[QuadActorId];
			if (!QuadActor || QuadActor->IsDestroyed() || QuadActorId == Id)
			{
				continue;
			}

			CollisionResult Result;
			if (Actor->Collide(*QuadActor, Result))
			{
				Actor->OnActorHit(QuadActor, Result);
			}
		}
	}
}

void pk::Scene::SetWindow(Window::WeakPtr InWindow)
{
	WindowPtr = std::move(InWindow);
	OnSetWindow();
}

Window::SharedPtr pk::Scene::GetWindow() const
{
	return WindowPtr.lock();
}

void pk::Scene::Add(const Actor::SharedPtr& InActor)
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

void pk::Scene::Add(const WidgetSharedPtr& InWidget)
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

void pk::Scene::Destroyer()
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

void pk::Scene::AddPendingActors()
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

void pk::Scene::UpdateActiveWidgets()
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

pk::Scene::~Scene() = default;

void pk::Scene::OnSetWindow()
{
	if (WindowPtr.expired())
	{
		return;
	}

	Projection = glm::ortho(0.f, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()), 0.f, -1.0f, 1.0f);
}

void pk::Scene::ClearWindow() const
{
	GetWindow()->ClearColor(Colors::LightBlack);
	GetWindow()->ClearFlags(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void pk::Scene::Clean()
{
	AddPendingActors();
	Destroyer();
	UpdateActiveWidgets();
	GetWindow()->CloseFrame();
}

void pk::Scene::UpdateDelta()
{
	CurrentTime = static_cast<float>(glfwGetTime());
	Delta = CurrentTime - OldTime;
	OldTime = CurrentTime;
	Fps = 1 / Delta;
}

void pk::Scene::RenderActors() const
{
	for (const ActorMapPair ActorPair : Actors)
	{
		ActorPair.second->Render();
	}
}

void pk::Scene::RenderWidgets() const
{
	for (const WidgetMapPair WidgetPair : ActiveWidgets)
	{
		WidgetPair.second->Render();
	}
}

void pk::Scene::HandleActorsInput(const float Delta) const
{
	for (const ActorMapPair ActorPair : Actors)
	{
		ActorPair.second->Input(IHandler, Delta);
	}
}

void pk::Scene::HandleWidgetInput(const float Delta) const
{
	for (const WidgetMapPair WidgetMap : ActiveWidgets)
	{
		WidgetMap.second->Input(IHandler, Delta);
	}
}
