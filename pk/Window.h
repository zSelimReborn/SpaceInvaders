#pragma once

#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <stdexcept>

struct GLFWwindow;

class Window
{
public:
	typedef std::shared_ptr<Window> SharedPtr;
	typedef std::weak_ptr<Window> WeakPtr;
	typedef std::unique_ptr<Window> UniquePtr;

	Window(const int _Width, const int _Height, const std::string& _Title);
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

	virtual ~Window();

	class Error : public std::runtime_error
	{
		using std::runtime_error::runtime_error;
	};

private:
	static void FrameBufferSizeCallback(GLFWwindow* Window, int _Width, int _Height);

	int Width;
	int Height;
	std::string Title;

	GLFWwindow* WindowPtr;
};

