#include <iostream>
#include "glad/glad.h"

#include "Assets.h"
#include "pk/AssetManager.h"
#include "pk/Window.h"
#include "pk/Scene.h"
#include "pk/Actor.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const std::string WINDOW_TITLE = "Space Invaders";

using namespace Assets;

int main(int argc, char** argv)
{
	Window::SharedPtr WindowPtr = std::make_shared<Window>(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
	Scene::SharedPtr ScenePtr = std::make_shared<Scene>(WindowPtr);

	glm::vec3 Location(800 / 2, 600 / 2, 1.0f);
	glm::vec3 Size(20.f, 20.f, 1.f);

	try
	{
		WindowPtr->Initialize();
		Shader::SharedPtr Shader = AssetManager::Get().LoadShader(Shaders::ShapeName, Shaders::ShapeVertexFile, Shaders::ShapeFragmentFile);
		Shader->Use();
		Shader->SetMatrix("projection", ScenePtr->GetProjection());

		Actor::SharedPtr Quad = std::make_shared<Actor>(Location, Size);
		Quad->SetShader(Shaders::ShapeName);
		Quad->SetColor(Colors::White);

		ScenePtr->Add(Quad);

		ScenePtr->Begin();
	}
	catch (const std::runtime_error& Error)
	{
		std::cout << "[ERROR]: " << Error.what() << "\n";
	}

	while (!ScenePtr->ShouldClose())
	{
		ScenePtr->Frame();
	}

	return 0;
}