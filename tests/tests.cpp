#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include "../src/Point.h"
#include "../src/Path.h"

#define EPSILON std::numeric_limits<float>::epsilon()

bool AlmostEquals(float a, float b) {
  return (std::fabs(a - b) < EPSILON);
}

using namespace ND$1Recognizer;

void TestPoints();

int main() {
  TestPoints();

  return 0;
}

void TestPoints() {
  // 1D points.
  {
    Point<1> a, b;
    a[0] = 0;
    b[0] = 0;
    assert (Distance(a, b) == 0);

    a[0] = 0;
    b[0] = 1;
    assert (Distance(a, b) == 1);

    a[0] = 2;
    b[0] = 1;
    assert (Distance(a, b) == 1);
  }
  // 2D points.
  {
    Point<2> a, b;
    a[0] = a[1] = 0;
    b[0] = b[1] = 0;
    assert (Distance(a, b) == 0);

    a[0] = a[1] = 0;
    b[0] = 0; b[1] = 1;
    assert (Distance(a, b) == 1);

    a[0] = a[1] = 0;
    b[0] = b[1] = 1;
    assert (AlmostEquals(Distance(a, b), std::sqrt(2)));
  }
  // 3D points.
  {
    Point<3> a, b;
    a[0] = a[1] = a[2] = 0;
    b[0] = b[1] = b[2] = 1;
    assert (AlmostEquals(Distance(a, b), std::sqrt(3)));
  }
}
