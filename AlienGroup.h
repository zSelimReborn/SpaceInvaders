#pragma once

#include <functional>
#include <vector>
#include <map>

#include "Alien.h"

class ProjectilePool;

enum class GroupState : std::uint8_t
{
	None,
	Moving,
	Stopped
};

class AlienGroup : public Actor
{
public:
	typedef std::shared_ptr<AlienGroup> SharedPtr;
	typedef std::vector<Alien::SharedPtr> AlienList;
	typedef std::map<AlienType, std::string> ConfigMap;
	typedef std::pair<AlienType, std::string> ConfigMapPair;
	typedef std::function<void()> OnReachedPlayerDelegate;
	typedef std::function<void()> OnDefeatDelegate;

	static const int DEFAULT_NUM_ROWS_PER_TYPE;
	static const int DEFAULT_ALIEN_PER_ROW;
	static const float DEFAULT_TOP_OFFSET;
	static const float DEFAULT_MOVE_DELAY;
	static const float DEFAULT_SHOOT_MAX_COOLDOWN;
	static const float DEFAULT_SHOOT_MIN_COOLDOWN;
	static const float DEFAULT_H_MOVE_STEP;
	static const float DEFAULT_V_MOVE_STEP;
	static const float DEFAULT_H_DISTANCE;
	static const float DEFAULT_V_DISTANCE;
	static const glm::vec3 DEFAULT_ALIEN_SIZE;

	AlienGroup();

	GroupState GetState() const;
	int GetNumRowsPerType() const;
	int GetNumRowsTotal() const;
	int GetNumAlienPerRow() const;
	float GetTopOffset() const;
	float GetMoveDelay() const;
	float GetShootMaxCooldown() const;
	float GetShootMinCooldown() const;
	float GetHorizontalMoveStep() const;
	float GetVerticalMoveStep() const;
	float GetHorizontalDistance() const;
	float GetVerticalDistance() const;
	glm::vec3 GetAlienSize() const;

	void SetNumRowsPerType(int InNum);
	void SetNumAlienPerRow(int InNum);
	void SetTopOffset(float InOffset);
	void SetMoveDelay(float InMoveDelay);
	void SetShootMaxCooldown(float InShootMaxCooldown);
	void SetShootMinCooldown(float InShootMinCooldown);
	void SetHorizontalMoveStep(float InMoveStep);
	void SetVerticalMoveStep(float InMoveStep);
	void SetHorizontalDistance(float InDistance);
	void SetVerticalDistance(float InDistance);
	void SetAlienSize(const glm::vec3& InAlienSize);

	void SetProjectilePool(const std::shared_ptr<ProjectilePool>& InProjectilePool);

	void LoadConfig() override;
	void Begin() override;
	void Update(const float Delta) override;

	void StartGroup();

	void AddOnReachedPlayerDelegate(const OnReachedPlayerDelegate& InFunction);
	void AddOnDefeatDelegate(const OnDefeatDelegate& InFunction);

	void HideBoard() const;

private:
	void BuildMatrix();
	void BuildMatrixPerType(AlienType Type);
	bool ReachedEnd() const;
	bool ReachedPlayer() const;
	void UpdateOuterColsAndRow();
	void MoveAliens(const float Delta);

	void GenerateShootCooldown();
	void Shoot() const;

	void UpdateShootCooldown(const float Delta);
	void UpdateMoveDelay(const float Delta);
	void UpdateAliveAliens();

	void NotifyReachedPlayer() const;
	void NotifyDefeat() const;

	bool bRightDirection;
	bool bGoDown;
	int NumRowsPerType;
	int NumAlienPerRow;
	int OuterLeftCol;
	int OuterRightCol;
	int LastRow;
	float TopOffset;
	float MoveDelay;
	float CurrentDelay;
	float ShootMaxCooldown;
	float ShootMinCooldown;
	float SelectedShootCooldown;
	float CurrentShootCooldown;
	float HorizontalMoveStep;
	float VerticalMoveStep;
	float HorizontalDistance;
	float VerticalDistance;
	glm::vec3 AlienSize;

	std::shared_ptr<ProjectilePool> ProjectilePoolPtr;

	AlienList AllAliens;
	std::vector<int> AliveAliensIdx;

	ConfigMap ConfigTypeMapping;

	std::vector<OnReachedPlayerDelegate> OnReachedPlayerFunctions;
	std::vector<OnDefeatDelegate> OnDefeatFunctions;

	GroupState State;
};