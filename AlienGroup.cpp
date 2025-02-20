#include "AlienGroup.h"

#include <iostream>

#include "Assets.h"
#include "pk/Scene.h"
#include "pk/SettingsReader.h"

const int AlienGroup::DEFAULT_NUM_ROWS_PER_TYPE = 2;
const int AlienGroup::DEFAULT_ALIEN_PER_ROW = 11;
const float AlienGroup::DEFAULT_MOVE_DELAY = 2.f;
const float AlienGroup::DEFAULT_CHANCE_TO_SHOOT = 0.4f;
const float AlienGroup::DEFAULT_H_MOVE_STEP = 10.f;
const float AlienGroup::DEFAULT_V_MOVE_STEP = 5.f;
const float AlienGroup::DEFAULT_H_DISTANCE = 20.f;
const float AlienGroup::DEFAULT_V_DISTANCE = 20.f;
const glm::vec3 AlienGroup::DEFAULT_ALIEN_SIZE = glm::vec3(10.f, 10.f, 1.f);

AlienGroup::AlienGroup()
	: bRightDirection(true), bGoDown(false),
		NumRowsPerType(DEFAULT_NUM_ROWS_PER_TYPE), NumAlienPerRow(DEFAULT_ALIEN_PER_ROW),
		OuterLeftCol(0), OuterRightCol(DEFAULT_ALIEN_PER_ROW - 1),
		MoveDelay(DEFAULT_MOVE_DELAY), CurrentDelay(0.f), ChanceToShoot(DEFAULT_CHANCE_TO_SHOOT),
		HorizontalMoveStep(DEFAULT_H_MOVE_STEP), VerticalMoveStep(DEFAULT_V_MOVE_STEP),
		HorizontalDistance(DEFAULT_H_DISTANCE), VerticalDistance(DEFAULT_V_DISTANCE),
		AlienSize(DEFAULT_ALIEN_SIZE)
{
	ConfigTypeMapping.insert(ConfigMapPair(AlienType::Squid, Assets::Config::SquidFile));
	ConfigTypeMapping.insert(ConfigMapPair(AlienType::Crab, Assets::Config::CrabFile));
	ConfigTypeMapping.insert(ConfigMapPair(AlienType::Octopus, Assets::Config::OctopusFile));
}

int AlienGroup::GetNumRowsPerType() const
{
	return NumRowsPerType;
}

int AlienGroup::GetNumAlienPerRow() const
{
	return NumAlienPerRow;
}

float AlienGroup::GetMoveDelay() const
{
	return MoveDelay;
}

float AlienGroup::GetChanceToShoot() const
{
	return ChanceToShoot;
}

float AlienGroup::GetHorizontalMoveStep() const
{
	return HorizontalMoveStep;
}

float AlienGroup::GetVerticalMoveStep() const
{
	return VerticalMoveStep;
}

float AlienGroup::GetHorizontalDistance() const
{
	return HorizontalDistance;
}

float AlienGroup::GetVerticalDistance() const
{
	return VerticalDistance;
}

glm::vec3 AlienGroup::GetAlienSize() const
{
	return AlienSize;
}

void AlienGroup::SetNumRowsPerType(int InNum)
{
	NumRowsPerType = std::abs(InNum);
}

void AlienGroup::SetNumAlienPerRow(int InNum)
{
	NumAlienPerRow = std::abs(InNum);
}

void AlienGroup::SetMoveDelay(float InMoveDelay)
{
	MoveDelay = std::abs(InMoveDelay);
}

void AlienGroup::SetChanceToShoot(float InChanceToShoot)
{
	ChanceToShoot = std::abs(InChanceToShoot);
}

void AlienGroup::SetHorizontalMoveStep(float InMoveStep)
{
	HorizontalMoveStep = std::abs(InMoveStep);
}

void AlienGroup::SetVerticalMoveStep(float InMoveStep)
{
	VerticalMoveStep = std::abs(InMoveStep);
}

void AlienGroup::SetHorizontalDistance(float InDistance)
{
	HorizontalDistance = std::abs(InDistance);
}

void AlienGroup::SetVerticalDistance(float InDistance)
{
	VerticalDistance = std::abs(InDistance);
}

void AlienGroup::SetAlienSize(const glm::vec3& InAlienSize)
{
	AlienSize = glm::abs(InAlienSize);
}

void AlienGroup::SetProjectilePool(const std::shared_ptr<ProjectilePool>& InProjectilePool)
{
	ProjectilePoolPtr = InProjectilePool;
}

void AlienGroup::LoadConfig()
{
	Actor::LoadConfig();

	Settings::SharedConstPtr GroupSettings = SettingsReader::Load(GetConfigFile());
	if (GroupSettings == nullptr)
	{
		return;
	}

	int InNumRows, InNumAlien;
	float InMoveDelay, InChanceToShoot, InHMoveStep, InVMoveStep, InHDistance, InVDistance;
	glm::vec3 InAlienSize;
	GroupSettings->Get("NumRowsPerType", DEFAULT_NUM_ROWS_PER_TYPE, InNumRows);
	GroupSettings->Get("NumAlienPerRow", DEFAULT_ALIEN_PER_ROW, InNumAlien);
	GroupSettings->Get("MoveDelay", DEFAULT_MOVE_DELAY, InMoveDelay);
	GroupSettings->Get("ChanceToShoot", DEFAULT_CHANCE_TO_SHOOT, InChanceToShoot);
	GroupSettings->Get("HorizontalMoveStep", DEFAULT_H_MOVE_STEP, InHMoveStep);
	GroupSettings->Get("VerticalMoveStep", DEFAULT_V_MOVE_STEP, InVMoveStep);
	GroupSettings->Get("HorizontalDistance", DEFAULT_H_DISTANCE, InHDistance);
	GroupSettings->Get("VerticalDistance", DEFAULT_V_DISTANCE, InVDistance);
	GroupSettings->Get("AlienSize", InAlienSize);

	SetNumRowsPerType(InNumRows);
	SetNumAlienPerRow(InNumAlien);
	SetMoveDelay(InMoveDelay);
	SetChanceToShoot(InChanceToShoot);
	SetHorizontalMoveStep(InHMoveStep);
	SetVerticalMoveStep(InVMoveStep);
	SetHorizontalDistance(InHDistance);
	SetVerticalDistance(InVDistance);
	SetAlienSize(InAlienSize);
}

void AlienGroup::Begin()
{
	Actor::Begin();

	BuildMatrix();
	StartGroup();
}

void AlienGroup::Update(const float Delta)
{
	Actor::Update(Delta);
	CurrentDelay += Delta;
	if (CurrentDelay >= MoveDelay)
	{
		UpdateOuterCols();
		MoveAliens(Delta);
		CurrentDelay = 0.f;
	}
}

void AlienGroup::StartGroup()
{
	Scene::SharedPtr CurrentScene = GetScene();
	if (CurrentScene == nullptr)
	{
		return;
	}

	bRightDirection = true;
	bGoDown = false;
	CurrentDelay = 0.f;
	OuterLeftCol = 0;
	OuterRightCol = NumAlienPerRow - 1;

	const float Width = static_cast<float>(CurrentScene->GetScreenWidth());

	const float AlienPerRow = static_cast<float>(NumAlienPerRow);

	const float GroupRectangleWidth = (AlienSize.x * AlienPerRow) + (HorizontalDistance * AlienPerRow);
	const float StartX = (Width - GroupRectangleWidth) / 2;

	glm::vec3 CurrentLocation(0.f);
	CurrentLocation.y = (AlienSize.y * static_cast<float>(NumRowsPerType));
	CurrentLocation.x = StartX + (AlienSize.x / 2);

	for (const std::vector<Alien::SharedPtr>& Row : Matrix)
	{
		for (const Alien::SharedPtr& Alien : Row)
		{
			Alien->CancelDestroy();
			CurrentScene->Add(Alien);
			Alien->SetSize(AlienSize);
			Alien->SetLocation(CurrentLocation);

			CurrentLocation.x += HorizontalDistance + (AlienSize.x);
		}

		CurrentLocation.x = StartX + (AlienSize.x / 2);
		CurrentLocation.y += VerticalDistance + (AlienSize.y);
	}
}

void AlienGroup::BuildMatrix()
{
	BuildMatrixPerType(AlienType::Octopus);
	BuildMatrixPerType(AlienType::Crab);
	BuildMatrixPerType(AlienType::Squid);
}

void AlienGroup::BuildMatrixPerType(AlienType Type)
{
	for (int i = 0; i < NumRowsPerType; ++i)
	{
		std::vector<Alien::SharedPtr> Row;
		for (int j = 0; j < NumAlienPerRow; ++j)
		{
			Alien::SharedPtr NewAlien = std::make_shared<Alien>(Type);
			NewAlien->SetConfig(ConfigTypeMapping[Type]);
			NewAlien->SetShader(Assets::Shaders::ShapeName);
			NewAlien->SetProjectilePool(ProjectilePoolPtr);

			Row.push_back(NewAlien);
		}

		Matrix.push_back(Row);
	}
}

bool AlienGroup::ReachedEnd() const
{
	const Scene::SharedPtr CurrentScene = GetScene();
	if (CurrentScene == nullptr)
	{
		return false;
	}

	const float Width = static_cast<float>(CurrentScene->GetScreenWidth());
	const Alien::SharedPtr LeftAlien = Matrix[0][OuterLeftCol];
	const Alien::SharedPtr RightAlien = Matrix[0][OuterRightCol];
	const glm::vec3 LeftLocation = LeftAlien->GetLocation();
	const glm::vec3 RightLocation = RightAlien->GetLocation();

	if (!bRightDirection && LeftLocation.x - HorizontalMoveStep - (AlienSize.x) <= 0.f)
	{
		return true;
	}

	if (bRightDirection && RightLocation.x + HorizontalMoveStep + (AlienSize.x) >= Width)
	{
		return true;
	}

	return false;
}

void AlienGroup::UpdateOuterCols()
{
	for (int j = 0; j < NumAlienPerRow; ++j)
	{
		for (int i = 0; i < NumRowsPerType; ++i)
		{
			if (!Matrix[i][j]->IsDestroyed() && j < OuterLeftCol)
			{
				OuterLeftCol = j;
			}

			const int RightIndex = NumAlienPerRow - j - 1;
			if (!Matrix[i][RightIndex]->IsDestroyed() && RightIndex > OuterRightCol)
			{
				OuterRightCol = RightIndex;
			}
		}
	}
}

void AlienGroup::MoveAliens(const float Delta)
{
	glm::vec3 MoveOffset(0.f);
	if (bGoDown)
	{
		MoveOffset.y = VerticalMoveStep;
		bGoDown = false;
	}
	else if (bRightDirection)
	{
		MoveOffset.x = HorizontalMoveStep;
	}
	else
	{
		MoveOffset.x = -HorizontalMoveStep;
	}

	for (const std::vector<Alien::SharedPtr>& Row : Matrix)
	{
		for (const Alien::SharedPtr& Alien : Row)
		{
			const glm::vec3 NewLocation = Alien->GetLocation() + MoveOffset;
			Alien->SetLocation(NewLocation);
		}
	}

	if (ReachedEnd())
	{
		bRightDirection = !bRightDirection;
		bGoDown = true;
	}
}
