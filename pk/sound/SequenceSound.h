#pragma once
#include <vector>

#include "ISound.h"

namespace pk
{
	class SequenceSound : public ISound
	{
	public:
		SequenceSound();
		SequenceSound(const std::vector<std::string>& InPaths);

		void Add(const std::string& InPath);

		std::string GetPath() override;

	private:
		int CurrentIndex;
		std::vector<std::string> Paths;
	};
}