#include "QuadTree.h"

#include <iostream>

QuadBox::QuadBox() = default;

QuadBox::QuadBox(const glm::vec3& InOrigin, float InWidth, float InHeight)
	: Origin(InOrigin), Width(InWidth), Height(InHeight) { }

void QuadBox::Set(const glm::vec3& InOrigin, float InWidth, float InHeight)
{
	Origin = InOrigin;
	Width = InWidth;
	Height = InHeight;
}

bool QuadBox::Contains(const glm::vec3& InLocation) const
{
	const float Right = Origin.x + Width;
	const float Bottom =  Origin.y + Height;
	return (InLocation.x >= Origin.x && InLocation.x <= Right) &&
		(InLocation.y >= Origin.y && InLocation.y <= Bottom);
}

QuadEntity::QuadEntity() = default;

QuadEntity::QuadEntity(int InEntity, const glm::vec3& InLocation)
	: Entity(InEntity), Location(InLocation) { }

QuadTree::QuadTree() = default;

QuadTree::QuadTree(const QuadBox& InBox)
	: bDivided(false), Box(InBox), CurrentLevel(0)
{
	Entities.clear();
}

QuadTree::QuadTree(const glm::vec3& InOrigin, float InWidth, float InHeight)
	: QuadTree(QuadBox(InOrigin, InWidth, InHeight)) { }

void QuadTree::Insert(int Entity, const glm::vec3& Location)
{
	Insert(QuadEntity(Entity, Location));
}

bool QuadTree::Contains(const glm::vec3& InLocation) const
{
	return Box.Contains(InLocation);
}

QuadTree* QuadTree::Search(const glm::vec3& InLocation)
{
	if (!Contains(InLocation))
	{
		return nullptr;
	}

	if (!bDivided)
	{
		return this;
	}

	if (TopLeft != nullptr && TopLeft->Contains(InLocation))
	{
		return TopLeft->Search(InLocation);
	}

	if (TopRight != nullptr && TopRight->Contains(InLocation))
	{
		return TopRight->Search(InLocation);
	}

	if (BottomLeft != nullptr && BottomLeft->Contains(InLocation))
	{
		return BottomLeft->Search(InLocation);
	}

	if (BottomRight != nullptr && BottomRight->Contains(InLocation))
	{
		return BottomRight->Search(InLocation);
	}

	return nullptr;
}

std::vector<int> QuadTree::GetEntities() const
{
	std::vector<int> ActualEntities;
	ActualEntities.reserve(Entities.size());

	for (const QuadEntity& Entity : Entities)
	{
		ActualEntities.push_back(Entity.Entity);
	}

	return ActualEntities;
}

QuadTree::~QuadTree()
{
	std::cout << "Quad tree destroyed\n";
}

void QuadTree::Insert(const QuadEntity& InEntity)
{
	if (Entities.size() >= MAX_ENTITIES_PER_NODE && CurrentLevel < MAX_LVL)
	{
		Divide();
	}

	if (!bDivided)
	{
		Entities.emplace_back(InEntity);
	}
	else
	{
		QuadTree* Node = GetCorrectChild(InEntity);
		if (Node != nullptr)
		{
			Node->Insert(InEntity);
		}
	}
}

QuadTree* QuadTree::GetCorrectChild(const QuadEntity& InEntity) const
{
	if (TopLeft != nullptr && TopLeft->Contains(InEntity.Location))
	{
		return TopLeft.get();
	}

	if (TopRight != nullptr && TopRight->Contains(InEntity.Location))
	{
		return TopRight.get();
	}

	if (BottomLeft != nullptr && BottomLeft->Contains(InEntity.Location))
	{
		return BottomLeft.get();
	}

	if (BottomRight != nullptr && BottomRight->Contains(InEntity.Location))
	{
		return BottomRight.get();
	}

	return nullptr;
}

void QuadTree::Divide()
{
	const float HalfWidth = Box.Width / 2;
	const float HalfHeight = Box.Height / 2;

	const glm::vec3 TopLeftOrigin = Box.Origin;
	const glm::vec3 TopRightOrigin = glm::vec3(Box.Origin.x + HalfWidth, Box.Origin.y, Box.Origin.z);
	const glm::vec3 BottomLeftOrigin = glm::vec3(Box.Origin.x, Box.Origin.y + HalfHeight, Box.Origin.z);
	const glm::vec3 BottomRightOrigin = glm::vec3(Box.Origin.x + HalfWidth, Box.Origin.y + HalfHeight, Box.Origin.z);

	TopLeft = std::make_unique<QuadTree>(TopLeftOrigin, HalfWidth, HalfHeight);
	TopRight = std::make_unique<QuadTree>(TopRightOrigin, HalfWidth, HalfHeight);
	BottomLeft = std::make_unique<QuadTree>(BottomLeftOrigin, HalfWidth, HalfHeight);
	BottomRight = std::make_unique<QuadTree>(BottomRightOrigin, HalfWidth, HalfHeight);

	TopLeft->CurrentLevel = CurrentLevel + 1;
	TopRight->CurrentLevel = CurrentLevel + 1;
	BottomLeft->CurrentLevel = CurrentLevel + 1;
	BottomRight->CurrentLevel = CurrentLevel + 1;

	// Insert in all children to ensure overlapping objects are detected
	for (const QuadEntity& Entity : Entities)
	{
		QuadTree* Node = GetCorrectChild(Entity);
		if (Node != nullptr)
		{
			Node->Insert(Entity);
		}
	}

	Entities.clear();
	bDivided = true;
}

