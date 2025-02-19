#pragma once

#include <vector>
#include <map>

#include "Alien.h"

class ProjectilePool;

class AlienGroup : public Actor
{
public:
	typedef std::shared_ptr<AlienGroup> SharedPtr;
	typedef std::vector<std::vector<Alien::SharedPtr>> AlienMatrix;
	typedef std::map<AlienType, std::string> ConfigMap;
	typedef std::pair<AlienType, std::string> ConfigMapPair;

	static const int DEFAULT_NUM_ROWS_PER_TYPE;
	static const int DEFAULT_ALIEN_PER_ROW;
	static const float DEFAULT_MOVE_DELAY;
	static const float DEFAULT_CHANCE_TO_SHOOT;
	static const float DEFAULT_H_MOVE_STEP;
	static const float DEFAULT_V_MOVE_STEP;
	static const glm::vec3 DEFAULT_ALIEN_SIZE;

	AlienGroup();

	int GetNumRowsPerType() const;
	int GetNumAlienPerRow() const;
	float GetMoveDelay() const;
	float GetChanceToShoot() const;
	float GetHorizontalMoveStep() const;
	float GetVerticalMoveStep() const;
	glm::vec3 GetAlienSize() const;

	void SetNumRowsPerType(int InNum);
	void SetNumAlienPerRow(int InNum);
	void SetMoveDelay(float InMoveDelay);
	void SetChanceToShoot(float InChanceToShoot);
	void SetHorizontalMoveStep(float InMoveStep);
	void SetVerticalMoveStep(float InMoveStep);
	void SetAlienSize(const glm::vec3& InAlienSize);

	void SetProjectilePool(const std::shared_ptr<ProjectilePool>& InProjectilePool);

	void LoadConfig() override;
	void Begin() override;
	void Update(const float Delta) override;

	void StartGroup();

private:
	void BuildMatrix();
	void BuildMatrixPerType(AlienType Type);

	int NumRowsPerType;
	int NumAlienPerRow;
	float MoveDelay;
	float ChanceToShoot;
	float HorizontalMoveStep;
	float VerticalMoveStep;
	glm::vec3 AlienSize;

	std::shared_ptr<ProjectilePool> ProjectilePoolPtr;

	AlienMatrix Matrix;
	ConfigMap ConfigTypeMapping;
};