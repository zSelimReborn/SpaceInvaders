#pragma once

#include <memory>

namespace pk
{
	class Window;
	class Scene;

	class Engine
	{
	public:
		typedef std::shared_ptr<Window> WindowSharedPtr;
		typedef std::shared_ptr<Scene> SceneSharedPtr;

		Engine();
		Engine(Engine& InEngine) = delete;
		Engine(Engine&& InEngine) = delete;
		void operator=(const Engine& InEngine) = delete;
		void operator=(const Engine&& InEngine) = delete;

		void SetWindow(const WindowSharedPtr& InWindow);
		void SetCurrentScene(const SceneSharedPtr& InScene);

		void Begin() const;
		void Run() const;

		~Engine();

	private:
		WindowSharedPtr WindowPtr;
		SceneSharedPtr CurrentScene;
	};
};
