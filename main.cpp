#include <iostream>
#include "glad/glad.h"

#include "pk/Window.h"
#include "Game.h"

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

const std::string WINDOW_TITLE = "Space Invaders";

const glm::vec3 SHIP_SIZE(75.f, 20.f, 1.f);
const glm::vec3 SHIP_SIZE_HALF(SHIP_SIZE.x / 2.f, SHIP_SIZE.y / 2.f, 1.f);
const glm::vec3 SHIP_BASE_LOCATION(WINDOW_WIDTH / 2, WINDOW_HEIGHT - SHIP_SIZE_HALF.y, 1.0f);
const Transform SHIP_TRANSFORM(SHIP_BASE_LOCATION, SHIP_SIZE);
constexpr float SHIP_SPEED = 450.f;
constexpr float SHIP_COOLDOWN = 1.f;

int main(int argc, char** argv)
{
	Window::SharedPtr WindowPtr = std::make_shared<Window>(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
	Game::SharedPtr GamePtr = std::make_shared<Game>(WindowPtr);

	GamePtr->Initialize(SHIP_TRANSFORM, SHIP_SPEED, SHIP_COOLDOWN);

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