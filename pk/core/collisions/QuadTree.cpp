#include "QuadTree.h"
#include "Constants.h"

#include <iostream>

#include "QuadPool.h"

QuadBox::QuadBox() = default;

QuadBox::QuadBox(const QuadBox& InBox)
{
	Set(InBox.Origin, InBox.Width, InBox.Height);
}

QuadBox::QuadBox(QuadBox&& InBox)
{
	Set(InBox.Origin, InBox.Width, InBox.Height);
}

QuadBox& QuadBox::operator=(const QuadBox& InBox)
{
	Set(InBox.Origin, InBox.Width, InBox.Height);
	return *this;
}

QuadBox& QuadBox::operator=(QuadBox&& InBox)
{
	Set(InBox.Origin, InBox.Width, InBox.Height);
	return *this;
}

QuadBox::QuadBox(const glm::vec3& InOrigin, float InWidth, float InHeight)
	: Origin(InOrigin), LooseOrigin(InOrigin),
		Width(InWidth), LooseWidth(InWidth),
		Height(InHeight), LooseHeight(InHeight)
{
	CalcLooseProperties();
}

void QuadBox::Set(const glm::vec3& InOrigin, float InWidth, float InHeight)
{
	Origin = InOrigin;
	Width = InWidth;
	Height = InHeight;

	CalcLooseProperties();
}

bool QuadBox::Contains(const glm::vec3& InLocation) const
{
	if (QUAD_LOOSE)
	{
		const float Right = LooseOrigin.x + LooseWidth;
		const float Bottom = LooseOrigin.y + LooseHeight;
		return (InLocation.x >= LooseOrigin.x && InLocation.x <= Right) &&
			(InLocation.y >= LooseOrigin.y && InLocation.y <= Bottom);
	}

	const float Right = Origin.x + Width;
	const float Bottom =  Origin.y + Height;
	return (InLocation.x >= Origin.x && InLocation.x <= Right) &&
		(InLocation.y >= Origin.y && InLocation.y <= Bottom);
}

void QuadBox::CalcLooseProperties()
{
	const float WidthLoosePercentage = (Width * QUAD_LOOSE_PERCENTAGE) / 100.f;
	const float HeightLoosePercentage = (Height * QUAD_LOOSE_PERCENTAGE) / 100.f;
	LooseWidth = Width + WidthLoosePercentage;
	LooseHeight = Height + HeightLoosePercentage;
	LooseOrigin = glm::vec3(Origin.x - (WidthLoosePercentage / 2.f), Origin.y - (HeightLoosePercentage / 2.f), Origin.z);
}

QuadEntity::QuadEntity() = default;

QuadEntity::QuadEntity(int InEntity, const glm::vec3& InLocation)
	: Entity(InEntity), Location(InLocation) { }

QuadTree::QuadTree()
	: bDivided(false), CurrentLevel(0),
		TopLeft(nullptr), TopRight(nullptr), BottomLeft(nullptr), BottomRight(nullptr)
{
}

QuadTree::QuadTree(const QuadBox& InBox)
	: QuadTree()
{
	Box = InBox;
	Entities.clear();
}

QuadTree::QuadTree(const glm::vec3& InOrigin, float InWidth, float InHeight)
	: QuadTree(QuadBox(InOrigin, InWidth, InHeight)) { }

void QuadTree::SetBox(const QuadBox& InBox)
{
	Box = InBox;
}

void QuadTree::SetBox(const glm::vec3& InOrigin, float InWidth, float InHeight)
{
	Box.Set(InOrigin, InWidth, InHeight);
}

void QuadTree::Reset(const QuadBox& InBox, int InLevel)
{
	SetBox(InBox);
	bDivided = false;
	Entities.clear();
	CurrentLevel = InLevel;
}

void QuadTree::Reset(const glm::vec3& InOrigin, float InWidth, float InHeight, int InLevel)
{
	Reset(QuadBox(InOrigin, InWidth, InHeight), InLevel);
}

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

QuadTree::~QuadTree() = default;

void QuadTree::Insert(const QuadEntity& InEntity)
{
	if (Entities.size() >= MAX_ENTITIES_PER_NODE 
		&& CurrentLevel < MAX_LVL 
		&& !QuadPool::Get().IsEmpty()
	)
	{
		Divide();
	}

	if (!bDivided)
	{
		Entities.emplace_back(InEntity);
	}
	else
	{
		InsertInChildren(InEntity);
	}
}

void QuadTree::InsertInChildren(const QuadEntity& InEntity) const
{
	if (TopLeft != nullptr && TopLeft->Contains(InEntity.Location))
	{
		TopLeft->Insert(InEntity);
	}

	if (TopRight != nullptr && TopRight->Contains(InEntity.Location))
	{
		TopRight->Insert(InEntity);
	}

	if (BottomLeft != nullptr && BottomLeft->Contains(InEntity.Location))
	{
		BottomLeft->Insert(InEntity);
	}

	if (BottomRight != nullptr && BottomRight->Contains(InEntity.Location))
	{
		BottomRight->Insert(InEntity);
	}
}

void QuadTree::Divide()
{
	const float HalfWidth = Box.Width / 2;
	const float HalfHeight = Box.Height / 2;

	const glm::vec3 TopLeftOrigin = Box.Origin;
	const glm::vec3 TopRightOrigin = glm::vec3(Box.Origin.x + HalfWidth, Box.Origin.y, Box.Origin.z);
	const glm::vec3 BottomLeftOrigin = glm::vec3(Box.Origin.x, Box.Origin.y + HalfHeight, Box.Origin.z);
	const glm::vec3 BottomRightOrigin = glm::vec3(Box.Origin.x + HalfWidth, Box.Origin.y + HalfHeight, Box.Origin.z);

	const int ChildrenLevel = CurrentLevel + 1;

	TopLeft = QuadPool::Get().GetQuadTree();
	TopLeft->Reset(TopLeftOrigin, HalfWidth, HalfHeight, ChildrenLevel);

	TopRight = QuadPool::Get().GetQuadTree();
	TopRight->Reset(TopRightOrigin, HalfWidth, HalfHeight, ChildrenLevel);

	BottomLeft = QuadPool::Get().GetQuadTree();
	BottomLeft->Reset(BottomLeftOrigin, HalfWidth, HalfHeight, ChildrenLevel);

	BottomRight = QuadPool::Get().GetQuadTree();
	BottomRight->Reset(BottomRightOrigin, HalfWidth, HalfHeight, ChildrenLevel);

	for (const QuadEntity& Entity : Entities)
	{
		InsertInChildren(Entity);
	}

	Entities.clear();
	bDivided = true;
}

