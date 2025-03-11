#pragma once

#include <memory>
#include <string>
#include <glm/detail/type_vec.hpp>

namespace pk
{
	class Scene;
	class InputHandler;

	enum class TextOrient : std::uint8_t
	{
		Left,
		Center,
		Right
	};

	class Widget
	{
	public:
		typedef std::shared_ptr<Widget> SharedPtr;
		typedef std::shared_ptr<Scene> SceneSharedPtr;
		typedef std::weak_ptr<Scene> SceneWeakPtr;

		Widget();

		int GetId() const;

		virtual void Construct();
		virtual void Render();
		virtual void Input(const InputHandler& Handler, const float Delta);

		void Activate();
		void Deactivate();
		bool IsActive() const;

		void SetScene(const SceneWeakPtr& InScene);
		SceneSharedPtr GetScene() const;

		virtual ~Widget() = default;
	protected:
		static void RenderText(const std::string& InFontName,
			const glm::vec2& StartLocation,
			const std::string& InText, TextOrient Orient,
			float Scale, const glm::vec4& InColor,
			float& OutWidth, float& OutHeight);

	private:
		bool bActive;
		int Id;

		SceneWeakPtr ScenePtr;

		friend class Scene;
	};
}