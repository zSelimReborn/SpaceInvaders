#pragma once
#include <vector>

#include "ISound.h"

namespace pk
{
	class RandomSound : public ISound
	{
	public:
		RandomSound();
		RandomSound(const std::vector<std::string>& InPaths);

		void Add(const std::string& InPath);

		std::string GetPath() override;

	private:
		std::vector<std::string> Paths;
	};
}