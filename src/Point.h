#pragma once

#include <cstddef>
#include <cmath>

namespace NDRecognizer {
template <int N>
class Point {
 public:
  int numDimensions() const { return N; }
  const float& operator[](std::size_t i) const { return dimensions_[i]; }
  float& operator[](std::size_t i) { return dimensions_[i]; }

  friend std::ostream& operator<<(std::ostream& out, const Point<N>& p) {
    out << "(";
    for (std::size_t i = 0; i < N - 1; ++i) {
      out << p.dimensions_[i] << ",";
    }
    out << p.dimensions_[N-1] << ")";
    return out;
  }

 private:
  float dimensions_[N];
};

template <int N>
float Distance(const Point<N>& a, const Point<N>& b) {
  float distance = 0;
  for (std::size_t i = 0; i < N; ++i) {
    float diff = a[i] - b[i];
    distance += diff * diff;
  }
  return std::sqrt(distance);
}
}
