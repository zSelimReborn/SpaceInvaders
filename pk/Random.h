#pragma once

#include <random>

class Random
{
public:
	typedef std::random_device Seed;
	typedef std::default_random_engine REngine;
	typedef std::uniform_int_distribution<int> IntDistribution;
	typedef std::uniform_real_distribution<float> FloatDistribution;

	static float Get(float Min, float Max);
	static int Get(int Min, int Max);
private:
	static Seed Device;
	static REngine Engine;
};