#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "pk/Common.h"
#include "pk/Window.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const std::string WINDOW_TITLE = "Space Invaders";

int main(int argc, char** argv)
{
	Window::SharedPtr WindowPtr = std::make_shared<Window>(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

	try
	{
		WindowPtr->Initialize();
	} catch (const std::runtime_error& Error)
	{
		std::cout << "[ERROR]: " << Error.what() << "\n";
	}

	while (!WindowPtr->ShouldClose())
	{
		WindowPtr->ClearColor(Colors::LightBlack);
		WindowPtr->ClearFlags(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (WindowPtr->IsPressed(GLFW_KEY_ESCAPE))
		{
			WindowPtr->ShouldClose(true);
		}

		WindowPtr->CloseFrame();
	}

	return 0;
}