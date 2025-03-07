#pragma once

#include "../../pk/core/world/Actor.h"

class Brick;

class Bunker : public Actor
{
public:
	typedef std::shared_ptr<Bunker> SharedPtr;
	typedef std::shared_ptr<Brick> BrickSharedPtr;
	typedef std::vector<BrickSharedPtr> BrickList;
	typedef std::vector<std::string> SchemeType;

	static const glm::vec3 DEFAULT_BRICK_SIZE;
	static const glm::vec4 DEFAULT_BRICK_COLOR;

	Bunker() = default;
	Bunker(const Transform& InTransform);
	Bunker(const glm::vec3& InLocation, const glm::vec3& InSize);

	void SetBrickSize(const glm::vec3& InSize);
	void SetBrickColor(const glm::vec4& InColor);

	glm::vec3 GetBrickSize() const;
	glm::vec4 GetBrickColor() const;

	void LoadConfig() override;
	void Begin() override;

	void Build() const;

	~Bunker() override = default;
private:
	void BuildScheme();
	void InitializeBricks();

	glm::vec3 BrickSize;
	glm::vec4 BrickColor;

	SchemeType Scheme;
	BrickList Bricks;
};
