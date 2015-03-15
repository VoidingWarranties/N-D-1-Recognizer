#include <cassert>
#include <cmath>
#include <iostream>
#include "../src/Point.h"
#include "../src/Path.h"
#include "../src/Utility.h"

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

    Point<1> min, max;
    a.boundingBox(min, max);
    assert (min[0] == 0 && max[0] == 0);

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

    a.boundingBox(min, max);
    assert (min[0] == 0 && max[0] == 1);

    c.boundingBox(min, max);
    assert (min[0] == 1 && max[0] == 1);

    Path<1> scaled_a = a.scale(100);
    assert(scaled_a[0][0] == 0 && scaled_a[1][0] == 100);

    for (int n : {2, 10, 64}) {
      Path<1> resampled_a = a.resample(n);
      assert(resampled_a.size() == n);
      float expected_value = 0;
      for (std::size_t i = 0; i < n; ++i) {
        assert (AlmostEquals(resampled_a[i][0], expected_value));
        expected_value += 1.f / (n - 1);
      }

      a.boundingBox(min, max);
      assert (min[0] == 0 && max[0] == 1);
    }
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

    Point<2> min, max;
    a.boundingBox(min, max);
    assert (min[0] == 0 && min[1] == 0);
    assert (max[0] == 0 && max[1] == 0);

    a.addPoint(p);
    for (int n : {2, 10, 64}) {
      Path<2> resampled_a = a.resample(n);
      assert(resampled_a.size() == n);
      float expected_value = 0;
      for (std::size_t i = 0; i < n; ++i) {
        assert(AlmostEquals(resampled_a[i][0], expected_value));
        assert(AlmostEquals(resampled_a[i][1], expected_value));
        expected_value += 1.f / (n - 1);
      }
    }

    a.boundingBox(min, max);
    assert (min[0] == 0 && min[1] == 0);
    assert (max[0] == 1 && max[1] == 1);

    Path<2> scaled_a = a.scale(100);
    assert (scaled_a[0][0] == 0 && scaled_a[0][1] == 0);
    assert (scaled_a[1][0] == 100 && scaled_a[1][1] == 100);
  }
}
