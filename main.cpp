#include <iostream>

#include "glad/glad.h"

#include "pk/Window.h"
#include "Assets.h"
#include "Game.h"
#include "pk/SettingsReader.h"

Window::SharedPtr CreateWindow();

constexpr int DEFAULT_WINDOW_WIDTH = 800;
constexpr int DEFAULT_WINDOW_HEIGHT = 600;

const std::string DEFAULT_WINDOW_TITLE = "Space Invaders";

int main(int argc, char** argv)
{
	Window::SharedPtr WindowPtr = CreateWindow();
	Game::SharedPtr GamePtr = std::make_shared<Game>(WindowPtr);

	try
	{
		WindowPtr->Initialize();
		GamePtr->Begin();
	}
	catch (const std::runtime_error& Error)
	{
		std::cout << "[ERROR]: " << Error.what() << "\n";
	}

	while (!GamePtr->ShouldClose())
	{
		GamePtr->Frame();
	}

	return 0;
}

Window::SharedPtr CreateWindow()
{
	int WindowWidth = DEFAULT_WINDOW_WIDTH, WindowHeight = DEFAULT_WINDOW_HEIGHT;
	std::string WindowTitle = DEFAULT_WINDOW_TITLE;

	Settings::SharedConstPtr WindowSetting = SettingsReader::Load(Assets::Config::WindowFile);
	if (WindowSetting != nullptr)
	{
		WindowSetting->Get("Width", DEFAULT_WINDOW_WIDTH, WindowWidth);
		WindowSetting->Get("Height", DEFAULT_WINDOW_HEIGHT, WindowHeight);
		WindowSetting->Get("Title", DEFAULT_WINDOW_TITLE, WindowTitle);
	}
	else
	{
		std::cout << "Unable to read Window configuration. Back to defaults\n";
	}

	Window::SharedPtr WindowPtr = std::make_shared<Window>(WindowWidth, WindowHeight, WindowTitle);
	return WindowPtr;
}
