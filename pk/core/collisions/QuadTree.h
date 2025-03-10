#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

constexpr bool QUAD_LOOSE = true;
constexpr int MAX_ENTITIES_PER_NODE = 2;
constexpr int MAX_LVL = 10;

struct QuadBox
{
	glm::vec3 Origin;
	float Width;
	float Height;

	QuadBox();
	QuadBox(const glm::vec3& InOrigin, float InWidth, float InHeight);
	void Set(const glm::vec3& InOrigin, float InWidth, float InHeight);
	bool Contains(const glm::vec3& InLocation) const;
};

struct QuadEntity
{
	int Entity;
	glm::vec3 Location;

	QuadEntity();
	QuadEntity(int InEntity, const glm::vec3& InLocation);
};

class QuadTree
{
public:
	QuadTree();
	QuadTree(const QuadBox& InBox);
	QuadTree(const glm::vec3& InOrigin, float InWidth, float InHeight);

	void Insert(int Entity, const glm::vec3& Location);
	bool Contains(const glm::vec3& InLocation) const;
	QuadTree* Search(const glm::vec3& InLocation);
	std::vector<int> GetEntities() const;

	~QuadTree();

private:
	void Insert(const QuadEntity& InEntity);
	QuadTree* GetCorrectChild(const QuadEntity& InEntity) const;
	void Divide();

	bool bDivided;
	QuadBox Box;
	std::vector<QuadEntity> Entities;
	int CurrentLevel;

	std::unique_ptr<QuadTree> TopLeft;
	std::unique_ptr<QuadTree> TopRight;
	std::unique_ptr<QuadTree> BottomLeft;
	std::unique_ptr<QuadTree> BottomRight;
};
