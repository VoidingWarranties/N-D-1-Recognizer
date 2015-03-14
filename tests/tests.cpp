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
void TestPaths();

int main() {
  TestPoints();
  TestPaths();

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

void TestPaths() {
  // 1D path.
  {
    Path<1> a, b;
    Point<1> p;
    p[0] = 0;
    a.addPoint(p);
    b.addPoint(p);
    assert (a.size() == b.size());
    assert (a.size() == 1);
    assert (a.length() == b.length());
    assert (a.length() == 0);
    assert (Distance(a, b) == 0);

    Path<1> c;
    p[0] = 1;
    c.addPoint(p);
    assert (Distance(a, c) == 1);

    a.addPoint(p);
    c.addPoint(p);
    assert (a.size() == c.size());
    assert (a.size() == 2);
    assert (a.length() == 1);
    assert (c.length() == 0);
  }
  // 2D path.
  {
    Path<2> a, b;
    Point<2> p;
    p[0] = p[1] = 0;
    a.addPoint(p);
    p[0] = p[1] = 1;
    b.addPoint(p);
    assert (AlmostEquals(Distance(a, b), std::sqrt(2)));
  }
}
