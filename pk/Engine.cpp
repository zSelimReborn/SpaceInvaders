#include "Engine.h"

#include <iostream>

#include "core/world/Scene.h"

using namespace pk;

Engine::Engine() = default;

void Engine::SetWindow(const WindowSharedPtr& InWindow)
{
	WindowPtr = InWindow;
}

void Engine::SetCurrentScene(const SceneSharedPtr& InScene)
{
	CurrentScene = InScene;
	CurrentScene->SetWindow(WindowPtr);
}

void Engine::Begin() const
{
	CurrentScene->Begin();
}

void Engine::Run() const
{
	if (CurrentScene == nullptr)
	{
		std::cout << "[Engine] - No Scene selected, cannot run.\n";
		return;
	}

	while (!CurrentScene->ShouldClose())
	{
		CurrentScene->Frame();
	}
}

Engine::~Engine() = default;
