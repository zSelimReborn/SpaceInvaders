#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace pk
{
	struct QuadBox
	{
		glm::vec3 Origin;
		glm::vec3 LooseOrigin;
		float Width;
		float LooseWidth;
		float Height;
		float LooseHeight;

		QuadBox();
		QuadBox(const QuadBox& InBox);
		QuadBox(QuadBox&& InBox);
		QuadBox(const glm::vec3& InOrigin, float InWidth, float InHeight);
		QuadBox& operator=(const QuadBox& InBox);
		QuadBox& operator=(QuadBox&& InBox);
		void Set(const glm::vec3& InOrigin, float InWidth, float InHeight);
		bool LooseContains(const glm::vec3& InLocation) const;
		bool StrictContains(const glm::vec3& InLocation) const;
		void CalcLooseProperties();
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

		void SetBox(const QuadBox& InBox);
		void SetBox(const glm::vec3& InOrigin, float InWidth, float InHeight);
		void Reset(const QuadBox& InBox, int InLevel);
		void Reset(const glm::vec3& InOrigin, float InWidth, float InHeight, int InLevel);
		void Insert(int Entity, const glm::vec3& Location);
		bool LooseContains(const glm::vec3& InLocation) const;
		bool StrictContains(const glm::vec3& InLocation) const;
		QuadTree* Search(const glm::vec3& InLocation);
		std::vector<int> GetEntities() const;

		~QuadTree();

	private:
		void Insert(const QuadEntity& InEntity);
		void InsertInChildren(const QuadEntity& InEntity) const;
		void Divide();

		bool bDivided;
		QuadBox Box;
		std::vector<QuadEntity> Entities;
		int CurrentLevel;

		QuadTree* TopLeft;
		QuadTree* TopRight;
		QuadTree* BottomLeft;
		QuadTree* BottomRight;
	};
}
