#include "AlienGroup.h"

#include "Assets.h"
#include "pk/Scene.h"
#include "pk/SettingsReader.h"

const int AlienGroup::DEFAULT_NUM_ROWS_PER_TYPE = 2;
const int AlienGroup::DEFAULT_ALIEN_PER_ROW = 11;
const float AlienGroup::DEFAULT_MOVE_DELAY = 2.f;
const float AlienGroup::DEFAULT_CHANCE_TO_SHOOT = 0.4f;
const float AlienGroup::DEFAULT_H_MOVE_STEP = 10.f;
const float AlienGroup::DEFAULT_V_MOVE_STEP = 5.f;
const glm::vec3 AlienGroup::DEFAULT_ALIEN_SIZE = glm::vec3(10.f, 10.f, 1.f);

AlienGroup::AlienGroup()
	: NumRowsPerType(DEFAULT_NUM_ROWS_PER_TYPE), NumAlienPerRow(DEFAULT_ALIEN_PER_ROW),
		MoveDelay(DEFAULT_MOVE_DELAY), ChanceToShoot(DEFAULT_CHANCE_TO_SHOOT),
		HorizontalMoveStep(DEFAULT_H_MOVE_STEP), VerticalMoveStep(DEFAULT_V_MOVE_STEP),
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
	float InMoveDelay, InChanceToShoot, InHMoveStep, InVMoveStep;
	glm::vec3 InAlienSize;
	GroupSettings->Get("NumRowsPerType", DEFAULT_NUM_ROWS_PER_TYPE, InNumRows);
	GroupSettings->Get("NumAlienPerRow", DEFAULT_ALIEN_PER_ROW, InNumAlien);
	GroupSettings->Get("MoveDelay", DEFAULT_MOVE_DELAY, InMoveDelay);
	GroupSettings->Get("ChanceToShoot", DEFAULT_CHANCE_TO_SHOOT, InChanceToShoot);
	GroupSettings->Get("HorizontalMoveStep", DEFAULT_H_MOVE_STEP, InHMoveStep);
	GroupSettings->Get("VerticalMoveStep", DEFAULT_V_MOVE_STEP, InVMoveStep);
	GroupSettings->Get("AlienSize", InAlienSize);

	SetNumRowsPerType(InNumRows);
	SetNumAlienPerRow(InNumAlien);
	SetMoveDelay(InMoveDelay);
	SetChanceToShoot(InChanceToShoot);
	SetHorizontalMoveStep(InHMoveStep);
	SetVerticalMoveStep(InVMoveStep);
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
}

void AlienGroup::StartGroup()
{
	Scene::SharedPtr CurrentScene = GetScene();
	if (CurrentScene == nullptr)
	{
		return;
	}

	const float Width = static_cast<float>(CurrentScene->GetScreenWidth());
	const float Height = static_cast<float>(CurrentScene->GetScreenHeight());
	const glm::vec2 ScreenCenter(CurrentScene->GetScreenCenter());

	glm::vec3 CurrentLocation(0.f);
	CurrentLocation.y = ScreenCenter.y - (AlienSize.y * static_cast<float>(NumRowsPerType));
	CurrentLocation.x = ScreenCenter.x - (AlienSize.x * (static_cast<float>(NumAlienPerRow) / 2));

	for (const std::vector<Alien::SharedPtr>& Row : Matrix)
	{
		for (const Alien::SharedPtr& Alien : Row)
		{
			Alien->CancelDestroy();
			Alien->SetSize(AlienSize);
			Alien->SetLocation(CurrentLocation);

			CurrentLocation.x += HorizontalMoveStep + (AlienSize.x);
		}

		CurrentLocation.x = ScreenCenter.x - (AlienSize.x * (static_cast<float>(NumAlienPerRow) / 2));
		CurrentLocation.y += VerticalMoveStep * (AlienSize.y);
	}
}

void AlienGroup::BuildMatrix()
{
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

			GetScene()->Add(NewAlien);
			Row.push_back(NewAlien);
		}

		Matrix.push_back(Row);
	}
}
