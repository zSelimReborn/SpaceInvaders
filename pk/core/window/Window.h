#pragma once

#include <functional>
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <stdexcept>

struct GLFWwindow;

namespace pk
{
	class Window
	{
	public:
		typedef std::shared_ptr<Window> SharedPtr;
		typedef std::weak_ptr<Window> WeakPtr;
		typedef std::unique_ptr<Window> UniquePtr;
		typedef std::function<void()> OnCloseDelegate;

		Window(const int InWidth, const int InHeight, std::string InTitle);
		Window(const Window&) = delete;
		Window(const Window&&) = delete;
		void operator=(const Window&) = delete;
		void operator=(const Window&&) = delete;

		void Initialize();

		int GetWidth() const;
		int GetHeight() const;
		glm::vec2 GetScreenCenter() const;
		std::string GetTitle() const;
		GLFWwindow* GetWindow() const;

		void Maximize() const;
		void ShouldClose(int Value) const;
		int ShouldClose() const;
		void CloseFrame() const;
		bool IsPressed(int Key) const;
		bool IsReleased(int Key) const;

		void ClearColor(const glm::vec4& Color) const;
		void ClearFlags(int Flags) const;

		void SetBlendFunction(int Key, int Value) const;

		void SetInputMode(const int Mode, const int Value) const;

		void SetIcon(unsigned char* IconBytes, int IconWidth, int IconHeight) const;

		void SetOnCloseFunction(const OnCloseDelegate& InDelegate);

		virtual ~Window();

		class Error : public std::runtime_error
		{
			using std::runtime_error::runtime_error;
		};

	private:
		void FrameBufferSizeCallback(GLFWwindow* InWindow, int InWidth, int InHeight);
		void OnCloseCallback(GLFWwindow* InWindow);

		int Width;
		int Height;
		std::string Title;

		GLFWwindow* WindowPtr;

		OnCloseDelegate OnCloseFunction;
	};
}