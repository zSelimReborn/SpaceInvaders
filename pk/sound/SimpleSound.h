#pragma once

#include "ISound.h"

namespace pk
{
	class SimpleSound : public ISound
	{
	public:
		SimpleSound(std::string InName);

		std::string GetPath() override;

	private:
		std::string Name;
	};
}