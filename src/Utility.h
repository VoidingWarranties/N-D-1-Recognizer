#pragma once

#include <cmath>

#define EPSILON std::numeric_limits<float>::epsilon()

namespace NDRecognizer {
bool AlmostEquals(float a, float b) { return (std::fabs(a - b) < EPSILON); }
}
