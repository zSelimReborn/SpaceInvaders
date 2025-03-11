#pragma once

#include <cmath>

constexpr bool QUAD_LOOSE = true;
constexpr float QUAD_LOOSE_PERCENTAGE = 10;
constexpr int MAX_ENTITIES_PER_NODE = 5;
constexpr int MAX_LVL = 4;
const int MAX_POOL_SIZE = static_cast<int>(std::pow(4, MAX_LVL));
