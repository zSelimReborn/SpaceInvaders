#pragma once

#include "pk/Actor.h"
#include "Alien.h"

class Secret : public Actor
{
public:
	static const float DEFAULT_SPAWN_TIME_MIN;
	static const float DEFAULT_SPAWN_TIME_MAX;
	static const float DEFAULT_ALIEN_SPEED;
	static const glm::vec3 DEFAULT_ALIEN_SIZE;

	Secret();

	float GetSpawnTimeMin() const;
	float GetSpawnTimeMax() const;
	float GetSelectedSpawnTime() const;
	float GetAlienSpeed() const;
	glm::vec3 GetAlienSize() const;

	void SetSpawnTimeMax(float InSpawnTime);
	void SetSpawnTimeMin(float InSpawnTime);
	void SetAlienSpeed(float InSpeed);
	void SetAlienSize(const glm::vec3& InSize);

	void LoadConfig() override;
	void Begin() override;
	void Update(const float Delta) override;

private:
	void SelectSpawnTime();
	void UpdateSpawnTime(const float Delta);
	void UpdateAlien();
	void CreateAlien();
	void SpawnAlien();

	bool bAlienActive;
	float SpawnTimeMin;
	float SpawnTimeMax;
	float SelectedSpawnTime;
	float CurrentSpawnTime;
	float AlienSpeed;
	glm::vec3 AlienSize;

	Alien::SharedPtr CurrentAlien;
};
