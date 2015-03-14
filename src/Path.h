#pragma once

#include <vector>
#include <cstddef>
#include <cassert>
#include "Point.h"

namespace ND$1Recognizer {
template <int N>
class Path {
 public:
  void addPoint(const Point<N>& p) { points_.push_back(p); }
  int size() const { return points_.size(); }
  float length() const;
  const Point<N>& operator[](std::size_t i) const { return points_[i]; }
  Point<N>& operator[](std::size_t i) { return points_[i]; }

 private:
  std::vector<Point<N> > points_;
};

template <int N>
float Path<N>::length() const {
  float distance = 0;
  for(std::size_t i = 1; i < points_.size(); ++i) {
    distance += Distance(points_[i-1], points_[i]);
  }
  return distance;
}

template <int N>
float Distance(const Path<N>& a, const Path<N>& b) {
  assert (a.size() == b.size());
  float distance = 0;
  for (std::size_t i = 0; i < a.size(); ++i) {
    distance += Distance(a[i], b[i]);
  }
  return distance / a.size();
}
}
