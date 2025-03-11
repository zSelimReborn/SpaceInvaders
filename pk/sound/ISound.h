#pragma once

#include <string>

namespace pk
{
	class ISound
	{
	public:
		virtual std::string GetPath() = 0;
		virtual ~ISound() = default;
	};
}