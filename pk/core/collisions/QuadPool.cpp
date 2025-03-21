#include "QuadPool.h"

#include <iostream>

#include "Constants.h"
#include "QuadTree.h"

using namespace pk;

QuadTree* QuadPool::GetQuadTree()
{
	// Should be an assert
	if (IsEmpty())
	{
		std::cerr << "QuadPool is full.\n";
		return nullptr;
	}

	QuadTree* Quad = Pool[NextIndex];
	NextIndex += 1;

	return Quad;
}

void QuadPool::Reset()
{
	NextIndex = 0;
}

bool QuadPool::IsEmpty() const
{
	return NextIndex >= Pool.size();
}

QuadPool::~QuadPool()
{
	for (QuadTree* Quad : Pool)
	{
		delete Quad;
	}

	Pool.clear();
}

QuadPool::QuadPool()
	: NextIndex(0)
{
	Pool.clear();
	Pool.reserve(MAX_POOL_SIZE);
	AddPool(MAX_POOL_SIZE);
}

void QuadPool::AddPool(int Size)
{
	for (int i = 0; i < Size; ++i)
	{
		Pool.push_back(new QuadTree());
	}
}
