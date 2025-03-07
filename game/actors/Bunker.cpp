#include "Bunker.h"

#include <iostream>

#include "Brick.h"
#include "../Assets.h"
#include "../../pk/core/world/Scene.h"
#include "../../pk/core/utils/ClassSettingsReader.h"

const glm::vec3 Bunker::DEFAULT_BRICK_SIZE = glm::vec3(15.f, 15.f, 1.f);
const glm::vec4 Bunker::DEFAULT_BRICK_COLOR = Colors::White;

Bunker::Bunker(const Transform& InTransform)
	: Actor(InTransform),
		BrickSize(DEFAULT_BRICK_SIZE), BrickColor(DEFAULT_BRICK_COLOR)
{
	BuildScheme();
}

Bunker::Bunker(const glm::vec3& InLocation, const glm::vec3& InSize)
	: Bunker(Transform(InLocation, InSize))
{
}

void Bunker::SetBrickSize(const glm::vec3& InSize)
{
	BrickSize = glm::abs(InSize);
}

void Bunker::SetBrickColor(const glm::vec4& InColor)
{
	BrickColor = glm::abs(InColor);
}

glm::vec3 Bunker::GetBrickSize() const
{
	return BrickSize;
}

glm::vec4 Bunker::GetBrickColor() const
{
	return BrickColor;
}

void Bunker::LoadConfig()
{
	Actor::LoadConfig();

	ClassSettings::SharedConstPtr BunkerSettings = ClassSettingsReader::Load(GetConfigFile());
	if (BunkerSettings == nullptr)
	{
		return;
	}

	glm::vec3 InBrickSize(DEFAULT_BRICK_SIZE);
	glm::vec4 InBrickColor(DEFAULT_BRICK_COLOR);
	BunkerSettings->Get("BrickSize", InBrickSize);
	BunkerSettings->Get("BrickColor", InBrickColor);

	SetBrickSize(InBrickSize);
	SetBrickColor(InBrickColor);
}

void Bunker::Begin()
{
	Actor::Begin();

	InitializeBricks();
	Build();
}

void Bunker::Build() const
{
	Scene::SharedPtr CurrentScene = GetScene();
	if (CurrentScene == nullptr)
	{
		std::cout << "Cannot build bunker. Empty scene\n";
		return;
	}

	const int Rows = static_cast<int>(Scheme.size());
	const int Cols = static_cast<int>(Scheme[0].length());

	const glm::vec3 BunkerCenter(GetLocation());

	const float StartX = ((static_cast<float>(Cols) / 2) * BrickSize.x) - BrickSize.x / 2;
	const float StartY = (static_cast<float>(Rows) / 2) * BrickSize.y;

	int BrickIndex = 0;
	glm::vec3 StartLocation(BunkerCenter.x - StartX, BunkerCenter.y - StartY, 1.f);
	for (const std::string& SchemeRow : Scheme)
	{
		for (int i = 0; i < SchemeRow.length(); ++i)
		{
			const char Char = SchemeRow[i];
			if (Char == '*')
			{
				Brick::SharedPtr CurrentBrick = Bricks[BrickIndex];
				CurrentBrick->CancelDestroy();
				CurrentBrick->SetLocation(StartLocation);
				CurrentScene->Add(CurrentBrick);
				BrickIndex++;
			}

			StartLocation.x += BrickSize.x;
			if ((i + 1) % Cols == 0)
			{
				StartLocation.x = BunkerCenter.x - StartX;
				StartLocation.y += BrickSize.y;
			}
		}
	}
}

void Bunker::BuildScheme()
{
	Scheme = {
		" **** ",
		"******",
		"******",
		"******",
		"**  **",
	};
}

void Bunker::InitializeBricks()
{
	Bricks.clear();

	const int Rows = 5;
	const int Cols = 6;
	int Totals = Rows * Cols;
	Totals -= Cols - 2 - 2;


	for (int i = 0; i < Totals; ++i)
	{
		Brick::SharedPtr InBrick = std::make_shared<Brick>();
		InBrick->SetShader(Assets::Shaders::ShapeName);
		InBrick->SetColor(BrickColor);
		InBrick->SetSize(BrickSize);
		Bricks.push_back(InBrick);
	}
}
