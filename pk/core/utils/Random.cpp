#include "Random.h"

using namespace pk;

std::random_device Random::Device{};
std::default_random_engine Random::Engine{ Device()};

float Random::Get(float Min, float Max)
{
	FloatDistribution Distribution(Min, Max);
	return Distribution(Engine);
}

int Random::Get(int Min, int Max)
{
	IntDistribution Distribution(Min, Max);
	return Distribution(Engine);
}
