#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <memory>
#include <string>

#include "Shader.h"


struct Particle
{
	typedef std::shared_ptr<Particle> SharedPtr;

	glm::vec3 Position;
	glm::vec3 Direction;
	glm::vec4 Color;
	float Life;
	float Speed;
	float OverrideScale;

	void Set(const glm::vec3& InPosition, const glm::vec3& InDirection, const glm::vec4& InColor, float InLife, float InSpeed, float InOverrideScale);
};

namespace ParticlePattern
{
	class Base
	{
	public:
		typedef std::shared_ptr<Base> SharedPtr;
		typedef std::vector<Particle::SharedPtr> ParticleList;

		Base(bool bInLoop, float InSpeed, float InLife, int InSpawnAmount, const glm::vec4& InColor);

		bool ShouldLoop() const;
		int GetSpawnAmount() const;
		float GetSpeed() const;
		float GetLife() const;
		glm::vec4 GetColor() const;

		virtual void Spawn(ParticleList& Pool, int& LastInactive, const glm::vec3& Position, const glm::vec3& Direction, float OverrideScale);

		virtual ~Base() = default;

	protected:
		static int NextInactive(ParticleList& Pool, int LastInactive);
		void SpawnParticle(const Particle::SharedPtr& NewParticle, const glm::vec3& Position, const glm::vec3& Direction, float OverrideScale) const;

	private:
		bool bLoop;
		int SpawnAmount;
		float Speed;
		float Life;
		glm::vec4 Color;
	};

	class Linear : public Base
	{
	public:
		Linear(float InSpeed, float InLife, int InSpawnAmount, const glm::vec4& InColor);
		void Spawn(ParticleList& Pool, int& LastInactive, const glm::vec3& Position, const glm::vec3& Direction, float OverrideScale) override;

		~Linear() override = default;
	};

	class Bounce : public Base
	{
	public:
		Bounce(float InSpeed, float InLife, int InSpawnAmount, const glm::vec4& InColor);
		void Spawn(ParticleList& Pool, int& LastInactive, const glm::vec3& Position, const glm::vec3& Direction, float OverrideScale) override;

		~Bounce() override = default;
	};
}

class Emitter
{
public:
	typedef std::shared_ptr<Emitter> SharedPtr;
	typedef std::vector<Particle::SharedPtr> ParticleList;

	Emitter(int InPoolCapacity, float InParticleScale, std::string InShaderName, std::string InTextureName, ParticlePattern::Base::SharedPtr InParticlePattern);

	void Spawn(const glm::vec3& Position, const glm::vec3& Direction, float OverrideScale);
	void Spawn(const glm::vec3& Position, const glm::vec3& Direction);
	void Spawn(const glm::vec3& Position, float OverrideScale);
	void Spawn(const glm::vec3& Position);

	void Update(float Delta, const glm::vec3& Position, const glm::vec3& Direction);
	void Update(float Delta);

	void Render() const;
	void Reset();

	void SetParticleScale(float InScale);
	float GetParticleScale() const;

	~Emitter();

private:
	void InitializePool();

	float ParticleScale;

	ParticleList Pool;
	int LastInactive;
	int PoolCapacity;

	std::string ParticleShaderName;
	std::string ParticleTextureName;

	ParticlePattern::Base::SharedPtr ParticlePattern;
};