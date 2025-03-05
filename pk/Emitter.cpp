#include "Emitter.h"

#include "Shader.h"
#include "Texture.h"

#include <ctime>
#include <cstdlib>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include "AssetManager.h"
#include "Common.h"
#include "Renderer.h"

void Particle::Set(const glm::vec3& InPosition, const glm::vec3& InDirection, const glm::vec4& InColor, float InLife, float InSpeed, float InOverrideScale)
{
	Position = InPosition;
	Direction = InDirection;
	Color = InColor;
	Life = InLife;
	OverrideScale = InOverrideScale;
	Speed = InSpeed;
}

ParticlePattern::Base::Base(bool bInLoop, float InSpeed, float InLife, int InSpawnAmount, const glm::vec4& InColor)
	: bLoop(bInLoop), SpawnAmount(InSpawnAmount), Speed(InSpeed), Life(InLife), Color(InColor)
{
}

bool ParticlePattern::Base::ShouldLoop() const
{
	return bLoop;
}

float ParticlePattern::Base::GetSpeed() const
{
	return Speed;
}

float ParticlePattern::Base::GetLife() const
{
	return Life;
}

glm::vec4 ParticlePattern::Base::GetColor() const
{
	return Color;
}

int ParticlePattern::Base::GetSpawnAmount() const
{
	return SpawnAmount;
}

void ParticlePattern::Base::Spawn(ParticleList& Pool, int& LastInactive, const glm::vec3& Position,
	const glm::vec3& Direction, float OverrideScale)
{
}

int ParticlePattern::Base::NextInactive(ParticleList& Pool, const int LastInactive)
{
	for (int i = LastInactive; i < Pool.size(); ++i)
	{
		if (Pool[i]->Life <= 0.f)
		{
			return i;
		}
	}

	for (int i = 0; i < LastInactive; ++i)
	{
		if (Pool[i]->Life <= 0.f)
		{
			return i;
		}
	}

	return 0;
}

void ParticlePattern::Base::SpawnParticle(const Particle::SharedPtr& NewParticle, const glm::vec3& Position, const glm::vec3& Direction, float OverrideScale) const
{
	if (NewParticle == nullptr)
	{
		return;
	}

	NewParticle->Set(Position, Direction, Color, Life, Speed, OverrideScale);
}

ParticlePattern::Linear::Linear(float InSpeed, float InLife, int InSpawnAmount, const glm::vec4& InColor)
	: Base(true, InSpeed, InLife, InSpawnAmount, InColor)
{
}

void ParticlePattern::Linear::Spawn(ParticleList& Pool, int& LastInactive, const glm::vec3& Position,
	const glm::vec3& Direction, float OverrideScale)
{
	for (int i = 0; i < GetSpawnAmount(); ++i)
	{
		LastInactive = NextInactive(Pool, LastInactive);
		SpawnParticle(Pool[LastInactive], Position, Direction, OverrideScale);
	}
}

ParticlePattern::Bounce::Bounce(float InSpeed, float InLife, int InSpawnAmount, const glm::vec4& InColor)
	: Base(false, InSpeed, InLife, InSpawnAmount, InColor)
{

}

void ParticlePattern::Bounce::Spawn(ParticleList& Pool, int& LastInactive, const glm::vec3& Position,
	const glm::vec3& Direction, float OverrideScale)
{
	const std::vector<glm::vec3> Compass = {
		glm::vec3(0.f, -1.f, 0.f), // Top
		glm::vec3(1.f, 0.f, 0.f), // Right
		glm::vec3(0.f, 1.f, 0.f), // Bottom
		glm::vec3(-1.f, 0.f, 0.f), // Left
	};

	float MaxDot = -1.f;
	int DirectionIndex = 0;
	const glm::vec3 NormalizedDirection = glm::normalize(Direction);
	for (int i = 0; i < Compass.size(); ++i)
	{
		const float CurrentDot = glm::dot(NormalizedDirection, Compass[i]);
		if (CurrentDot > MaxDot)
		{
			MaxDot = CurrentDot;
			DirectionIndex = i;
		}
	}

	const glm::vec3 MainDirection = Compass[DirectionIndex];
	const glm::vec3 ToAdd = (MainDirection.x == 0.f) ? glm::vec3(1.f, 0.f, 0.f) : glm::vec3(0.f, 1.f, 0.f);

	std::vector<glm::vec3> DirectionsToSpawn;
	DirectionsToSpawn.push_back(MainDirection);
	DirectionsToSpawn.push_back(MainDirection + ToAdd);
	DirectionsToSpawn.push_back(MainDirection - ToAdd);

	const int DirectionsCount = static_cast<int>(DirectionsToSpawn.size());

	for (int i = 0; i < GetSpawnAmount(); ++i)
	{
		LastInactive = NextInactive(Pool, LastInactive);
		SpawnParticle(Pool[LastInactive], Position, DirectionsToSpawn[i % DirectionsCount], OverrideScale);
	}
}

Emitter::Emitter(int InPoolCapacity, float InParticleScale, std::string InShaderName, std::string InTextureName, ParticlePattern::Base::SharedPtr InParticlePattern)
	: ParticleScale(InParticleScale),
		LastInactive(0), PoolCapacity(InPoolCapacity),
		ParticleShaderName(std::move(InShaderName)), ParticleTextureName(std::move(InTextureName)),
		ParticlePattern(std::move(InParticlePattern))
{
	InitializePool();
}

void Emitter::Spawn(const glm::vec3& Position, const glm::vec3& Direction, float OverrideScale)
{
	if (!ParticlePattern)
	{
		return;
	}

	ParticlePattern->Spawn(Pool, LastInactive, Position, Direction, OverrideScale);
}

void Emitter::Spawn(const glm::vec3& Position, float OverrideScale)
{
	Spawn(Position, glm::vec3(0.f), OverrideScale);
}

void Emitter::Spawn(const glm::vec3& Position, const glm::vec3& Direction)
{
	Spawn(Position, Direction, -1.f);
}

void Emitter::Spawn(const glm::vec3& Position)
{
	Spawn(Position, glm::vec3(0.f));
}

void Emitter::Update(float Delta, const glm::vec3& Position, const glm::vec3& Direction)
{
	if (!ParticlePattern)
	{
		return;
	}

	if (ParticlePattern->ShouldLoop())
	{
		ParticlePattern->Spawn(Pool, LastInactive, Position, Direction, -1.f);
	}

	const float ColorDecayFactor = 2.f / ParticlePattern->GetLife();
	for (const Particle::SharedPtr& CurrentParticle : Pool)
	{
		CurrentParticle->Life -= Delta;
		if (CurrentParticle->Life <= 0.f)
		{
			continue;
		}

		const glm::vec3 Velocity = CurrentParticle->Direction * CurrentParticle->Speed;
		CurrentParticle->Position += Velocity * Delta;
		CurrentParticle->Color.a -= ColorDecayFactor * Delta;
	}
}

void Emitter::Update(float Delta)
{
	Update(Delta, glm::vec3(0.f), glm::vec3(0.f));
}

void Emitter::Render() const
{
	const Shader::SharedPtr Shader = AssetManager::Get().GetShader(ParticleShaderName);
	const Texture::SharedPtr Texture = AssetManager::Get().GetTexture(ParticleTextureName);

	Renderer::Get().RenderParticleVfx(
		Pool,
		Shader,
		Texture,
		ParticleScale
	);
}

void Emitter::Reset()
{
	for (const Particle::SharedPtr& CurrentParticle : Pool)
	{
		CurrentParticle->Life = 0.f;
	}

	LastInactive = 0;
}

void Emitter::SetParticleScale(float InScale)
{
	ParticleScale = InScale;
}

float Emitter::GetParticleScale() const
{
	return ParticleScale;
}

Emitter::~Emitter()
{
	Pool.clear();
}

void Emitter::InitializePool()
{
	Pool.reserve(PoolCapacity);

	for (int i = 0; i < PoolCapacity; ++i)
	{
		Pool.push_back(std::make_shared<Particle>());
	}
}

