#include <iostream>

#include <glad/glad.h>
#include <stb_image.h>

#include "pk/core/window/Window.h"
#include "pk/core/utils/ClassSettingsReader.h"
#include "pk/Engine.h"

#include "game/Assets.h"
#include "game/scenes/Game.h"

Window::SharedPtr CreateWindow();

constexpr int DEFAULT_WINDOW_WIDTH = 800;
constexpr int DEFAULT_WINDOW_HEIGHT = 600;

const std::string DEFAULT_WINDOW_TITLE = "Space Invaders";

int main(int argc, char** argv)
{
	Engine CurrentEngine;
	try
	{
		Window::SharedPtr WindowPtr = CreateWindow();
		Game::SharedPtr GamePtr = std::make_shared<Game>();
		CurrentEngine.SetWindow(WindowPtr);
		CurrentEngine.SetCurrentScene(GamePtr);
		CurrentEngine.Begin();
	}
	catch (const std::runtime_error& Error)
	{
		std::cout << "[ENGINE]: Error during begin phase: " << Error.what() << "\n";
		return -1;
	}

	CurrentEngine.Run();
	return 0;
}

Window::SharedPtr CreateWindow()
{
	int WindowWidth = DEFAULT_WINDOW_WIDTH, WindowHeight = DEFAULT_WINDOW_HEIGHT;
	std::string WindowTitle = DEFAULT_WINDOW_TITLE;

	ClassSettings::SharedConstPtr WindowSetting = ClassSettingsReader::Load(Assets::Config::WindowFile);
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

	int IconWidth, IconHeight, IconChannels;
	unsigned char* IconData = stbi_load(Assets::Textures::WindowIcon.c_str(), &IconWidth, &IconHeight, &IconChannels, 0);

	Window::SharedPtr WindowPtr = std::make_shared<Window>(WindowWidth, WindowHeight, WindowTitle);
	WindowPtr->SetIcon(IconData, IconWidth, IconHeight);

	stbi_image_free(IconData);
	return WindowPtr;
}
