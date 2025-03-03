#include "Secret.h"

#include <iostream>

#include "Assets.h"
#include "pk/Random.h"
#include "pk/Scene.h"
#include "pk/SettingsReader.h"

const float Secret::DEFAULT_SPAWN_TIME_MIN = 5.f;
const float Secret::DEFAULT_SPAWN_TIME_MAX = 8.f;
const float Secret::DEFAULT_ALIEN_SPEED = 500.f;
const float Secret::DEFAULT_ALIEN_TOP_OFFSET = 10.f;
const glm::vec3 Secret::DEFAULT_ALIEN_SIZE = glm::vec3(40.f, 20.f, 1.f);

Secret::Secret()
	: Actor(),
		bAlienActive(false), SpawnTimeMin(DEFAULT_SPAWN_TIME_MIN), SpawnTimeMax(DEFAULT_SPAWN_TIME_MAX),
		SelectedSpawnTime(0.f), CurrentSpawnTime(0.f), TopOffset(DEFAULT_ALIEN_TOP_OFFSET),
		AlienSpeed(DEFAULT_ALIEN_SPEED), AlienSize(DEFAULT_ALIEN_SIZE)
{
}

float Secret::GetSpawnTimeMin() const
{
	return SpawnTimeMin;
}

float Secret::GetSpawnTimeMax() const
{
	return SpawnTimeMax;
}

float Secret::GetSelectedSpawnTime() const
{
	return SelectedSpawnTime;
}

float Secret::GetAlienSpeed() const
{
	return AlienSpeed;
}

float Secret::GetTopOffset() const
{
	return TopOffset;
}

glm::vec3 Secret::GetAlienSize() const
{
	return AlienSize;
}

void Secret::SetSpawnTimeMax(float InSpawnTime)
{
	SpawnTimeMax = std::abs(InSpawnTime);
	if (SpawnTimeMax <= SpawnTimeMin)
	{
		SpawnTimeMax += SpawnTimeMin;
	}
}

void Secret::SetSpawnTimeMin(float InSpawnTime)
{
	SpawnTimeMin = std::abs(InSpawnTime);
}

void Secret::SetAlienSpeed(float InSpeed)
{
	AlienSpeed = std::abs(InSpeed);
}

void Secret::SetTopOffset(float InTopOffset)
{
	TopOffset = std::abs(InTopOffset);
}

void Secret::SetAlienSize(const glm::vec3& InSize)
{
	AlienSize = glm::abs(InSize);
}

void Secret::LoadConfig()
{
	Actor::LoadConfig();

	Settings::SharedConstPtr SecretSettings = SettingsReader::Load(GetConfigFile());
	if (SecretSettings == nullptr)
	{
		return;
	}

	float InSpawnTimeMin, InSpawnTimeMax, InAlienSpeed, InTopOffset;
	glm::vec3 InAlienSize;
	SecretSettings->Get("SpawnTimeMin", DEFAULT_SPAWN_TIME_MIN, InSpawnTimeMin);
	SecretSettings->Get("SpawnTimeMax", DEFAULT_SPAWN_TIME_MAX, InSpawnTimeMax);
	SecretSettings->Get("AlienSpeed", DEFAULT_ALIEN_SPEED, InAlienSpeed);
	SecretSettings->Get("TopOffset", DEFAULT_ALIEN_TOP_OFFSET, InTopOffset);
	SecretSettings->Get("AlienSize", InAlienSize);

	SetSpawnTimeMin(InSpawnTimeMin);
	SetSpawnTimeMax(InSpawnTimeMax);
	SetAlienSpeed(InAlienSpeed);
	SetTopOffset(InTopOffset);
	SetAlienSize(InAlienSize);
}

void Secret::Begin()
{
	Actor::Begin();

	CreateAlien();
	SelectSpawnTime();
}

void Secret::Update(const float Delta)
{
	Actor::Update(Delta);

	UpdateAlien();
	UpdateSpawnTime(Delta);
}

void Secret::Reset()
{
	bAlienActive = false;
	CurrentAlien->Destroy();
	SelectSpawnTime();
}

void Secret::SelectSpawnTime()
{
	SelectedSpawnTime = Random::Get(SpawnTimeMin, SpawnTimeMax);
}

void Secret::UpdateSpawnTime(const float Delta)
{
	if (bAlienActive)
	{
		return;
	}

	CurrentSpawnTime += Delta;
	if (CurrentSpawnTime >= SelectedSpawnTime)
	{
		SpawnAlien();
		CurrentSpawnTime = 0.f;
	}
}

void Secret::UpdateAlien()
{
	if (!bAlienActive)
	{
		return;
	}

	if (!CurrentAlien || !CurrentAlien->IsInViewport() || CurrentAlien->IsDestroyed())
	{
		CurrentAlien->Destroy();
		bAlienActive = false;
		SelectSpawnTime();
	}
}

void Secret::CreateAlien()
{
	CurrentAlien = std::make_shared<Alien>(AlienType::Secret);
	CurrentAlien->SetConfig(Assets::Config::BonusAlienFile);
	CurrentAlien->SetShader(Assets::Shaders::SpriteName);
	CurrentAlien->SetTexture(Assets::Textures::SecretName);
	CurrentAlien->SetSize(AlienSize);
}

void Secret::SpawnAlien()
{
	const SceneSharedPtr CurrentScene = GetScene();
	if (CurrentScene == nullptr)
	{
		return;
	}

	const float WindowWidth = static_cast<float>(CurrentScene->GetScreenWidth());

	bool bRight = Random::Get(0, 1) == 1;
	const float LocationX = (bRight) ? 0 : WindowWidth;
	const float DirectionX = (bRight) ? 1.f : -1.f;

	const glm::vec3 Direction(DirectionX, 0.f, 0.f);
	const glm::vec3 Location(LocationX, (AlienSize.y / 2) + TopOffset, 0.f);
	const glm::vec3 Velocity = Direction * AlienSpeed;

	CurrentAlien->CancelDestroy();
	CurrentAlien->SetLocation(Location);
	CurrentAlien->SetVelocity(Velocity);

	CurrentScene->Add(CurrentAlien);
	bAlienActive = true;
}
