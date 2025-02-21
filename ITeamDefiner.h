#pragma once

#include <cstdint>

enum class Team : std::uint8_t
{
	Human,
	Alien
};

class ITeamDefiner
{
public:
	virtual void SetTeam(Team InTeam) = 0;
	virtual Team GetTeam() const = 0;
	virtual ~ITeamDefiner() = default;
};