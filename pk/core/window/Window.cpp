#include "Window.h"

#include <iostream>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

Window::Window(const int InWidth, const int InHeight, std::string InTitle)
	: Width(InWidth), Height(InHeight), Title(std::move(InTitle)), WindowPtr(nullptr)
{
    Initialize();
}

void Window::Initialize()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL major version 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL minor version 3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // OpenGL Core profile

    GLFWwindow* window = glfwCreateWindow(Width, Height, Title.c_str(), nullptr, nullptr);
    if (window == nullptr)
    {
        throw Error("Failed to create GLFW window");
    }

    WindowPtr = window;
    glfwMakeContextCurrent(WindowPtr);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw Error("Failed to initialize GLAD");
    }

    glfwSetWindowUserPointer(WindowPtr, this);
    glfwSetFramebufferSizeCallback(WindowPtr, [](GLFWwindow* InWindow, int InWidth, int InHeight)
	    {
	        static_cast<Window*>(glfwGetWindowUserPointer(InWindow))->FrameBufferSizeCallback(InWindow, InWidth, InHeight);
	    }
    );
    glfwSetWindowCloseCallback(WindowPtr, [](GLFWwindow* InWindow)
        {
            static_cast<Window*>(glfwGetWindowUserPointer(InWindow))->OnCloseCallback(InWindow);
        }
    );

    glEnable(GL_BLEND);
    SetBlendFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int Window::GetWidth() const
{
	return Width;
}

int Window::GetHeight() const
{
	return Height;
}

glm::vec2 Window::GetScreenCenter() const
{
    return {Width / 2, Height / 2};
}

std::string Window::GetTitle() const
{
	return Title;
}

GLFWwindow* Window::GetWindow() const
{
    return WindowPtr;
}

void Window::Maximize() const
{
    glfwMaximizeWindow(WindowPtr);
}

void Window::ShouldClose(int Value) const
{
    glfwSetWindowShouldClose(WindowPtr, Value);
}

int Window::ShouldClose() const
{
    return glfwWindowShouldClose(WindowPtr);
}

void Window::CloseFrame() const
{
    glfwSwapBuffers(WindowPtr);
    glfwPollEvents();
}

bool Window::IsPressed(int Key) const
{
    return glfwGetKey(WindowPtr, Key) == GLFW_PRESS;
}

bool Window::IsReleased(int Key) const
{
    return glfwGetKey(WindowPtr, Key) == GLFW_RELEASE;
}

void Window::ClearColor(const glm::vec4& Color) const
{
    glClearColor(Color.x, Color.y, Color.z, Color.w);
}

void Window::ClearFlags(int Flags) const
{
    glClear(Flags);
}

void Window::SetBlendFunction(int Key, int Value) const
{
    glBlendFunc(Key, Value);
}

void Window::SetInputMode(const int Mode, const int Value) const
{
	glfwSetInputMode(WindowPtr, Mode, Value);
}

void Window::SetIcon(unsigned char* IconBytes, int IconWidth, int IconHeight) const
{
    if (IconBytes == nullptr || WindowPtr == nullptr)
    {
        return;
    }

    GLFWimage Icon[1];
    Icon[0].pixels = IconBytes;
    Icon[0].width = IconWidth;
    Icon[0].height = IconHeight;
    glfwSetWindowIcon(WindowPtr, 1, Icon);
}

void Window::SetOnCloseFunction(const OnCloseDelegate& InDelegate)
{
    OnCloseFunction = InDelegate;
}

Window::~Window()
{
    glfwTerminate();
}

void Window::FrameBufferSizeCallback(GLFWwindow* InWindow, int InWidth, int InHeight)
{
    glad_glViewport(0, 0, InWidth, InHeight);
}

void Window::OnCloseCallback(GLFWwindow* InWindow)
{
    OnCloseFunction();
}
