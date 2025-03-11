#pragma once

#include <vector>

namespace pk
{
	class QuadTree;

	class QuadPool
	{
	public:
		// A POOL SHOULD NOT BE COPIED, TRUE?
		QuadPool(const QuadPool&) = delete;
		QuadPool(const QuadPool&&) = delete;
		void operator=(const QuadPool&) = delete;
		void operator=(const QuadPool&&) = delete;

		static QuadPool& Get()
		{
			static QuadPool Instance;
			return Instance;
		}

		QuadTree* GetQuadTree();
		void Reset();
		bool IsEmpty() const;

		~QuadPool();
	private:
		QuadPool();
		void AddPool(int Size);

		std::vector<QuadTree*> Pool;
		int NextIndex;
	};
}