#pragma once

#include <vector>
#include <deque>
#include <cstddef>
#include <cassert>
#include "Point.h"
#include "Utility.h"

namespace ND$1Recognizer {
template <int N>
class Path {
 public:
  void addPoint(const Point<N>& p) { points_.push_back(p); }
  int size() const { return points_.size(); }
  float length() const;
  const Point<N>& operator[](std::size_t i) const { return points_[i]; }
  Point<N>& operator[](std::size_t i) { return points_[i]; }
  Path<N> resample(int n) const;

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
Path<N> Path<N>::resample(int n) const {
  assert (points_.size() > 0);
  assert (n > 1);
  Path<N> resampled;
  resampled.addPoint(points_[0]);
  float I = length() / (n - 1);
  float D = 0;
  std::deque<Point<N> > points(points_.begin(), points_.end());

  Point<N> prev = points.front();
  points.pop_front();
  while (!points.empty()) {
    Point<N> current = points.front();
    float d = Distance(prev, current);
    if ((D + d) >= I || AlmostEquals(D + d, I)) {
      Point<N> q;
      for (std::size_t dimension = 0; dimension < N; ++dimension) {
        q[dimension] = prev[dimension] + ((I - D) / d) * (current[dimension] - prev[dimension]);
      }
      resampled.addPoint(q);
      points.push_front(q);
      prev = q;
      D = 0;
    } else {
      D += d;
      prev = current;
      points.pop_front();
    }
  }
  return resampled;
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
