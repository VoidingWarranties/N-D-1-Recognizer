#pragma once

#include <vector>
#include <deque>
#include <cstddef>
#include <cassert>
#include <limits>
#include "Point.h"
#include "Utility.h"

namespace NDRecognizer {
template <int N>
class Path {
 public:
  explicit Path(float uniform_scale_min_ratio = 0.25);

  void addPoint(const Point<N>& p) { points_.push_back(p); }
  int size() const { return points_.size(); }
  float length() const;
  const Point<N>& operator[](std::size_t i) const { return points_[i]; }
  Point<N>& operator[](std::size_t i) { return points_[i]; }
  Path<N> resample(int n) const;
  Point<N> centroid() const;
  void boundingBox(Point<N>& minCorner, Point<N>& maxCorner) const;
  Path<N> scale(float scale_size) const;
  Path<N> translateCentroidTo(const Point<N>& p) const;

  friend std::ostream& operator<<(std::ostream& out, const Path<N>& p) {
    for (const auto& point : p.points_) {
      out << point << "\n";
    }
    return out;
  }

 private:
  std::vector<Point<N> > points_;
  float uniform_scale_min_ratio_;
};

template <int N>
Path<N>::Path(float uniform_scale_min_ratio)
    : uniform_scale_min_ratio_(uniform_scale_min_ratio) {}

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
  assert (points_.size() > 1);
  assert (n > 1);
  Path<N> resampled;
  resampled.addPoint(points_[0]);
  float I = length() / (n - 1);
  assert (I > 0);
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
  // This is a kludge to fix a bug where the last point is not always added to
  // the resampled path.
  if (resampled.size() != n) {
    resampled.addPoint(points_.back());
  }
  return resampled;
}

template <int N>
Point<N> Path<N>::centroid() const {
  Point<N> centroid_point;
  for (std::size_t i = 0; i < N; ++i) {
    centroid_point[i] = 0;
  }
  for (const auto& point : points_) {
    for (std::size_t i = 0; i < N; ++i) {
      centroid_point[i] += point[i] / float(points_.size());
    }
  }
  return centroid_point;
}

template <int N>
void Path<N>::boundingBox(Point<N>& minCorner, Point<N>& maxCorner) const {
  assert (points_.size() > 0);
  for (size_t i = 0; i < N; ++i) {
    minCorner[i] = std::numeric_limits<float>::max();
    // TODO: is there a better min value for floats?
    maxCorner[i] = -1 * std::numeric_limits<float>::max();
  }
  for (const auto& point : points_) {
    for (std::size_t i = 0; i < N; ++i) {
      if (point[i] < minCorner[i]) {
        minCorner[i] = point[i];
      }
      if (point[i] > maxCorner[i]) {
        maxCorner[i] = point[i];
      }
    }
  }
}

template <int N>
Path<N> Path<N>::scale(float scale_size) const {
  Path<N> scaled_path;
  Point<N> min, max;
  boundingBox(min, max);

  float max_length = 0;
  float min_length = -1;
  for (std::size_t i = 0; i < N; ++i) {
    float length = max[i] - min[i];
    if (length > max_length) {
      max_length = length;
    }
    if (length < min_length || min_length == -1) {
      min_length = length;
    }
  }

  if (min_length / max_length < uniform_scale_min_ratio_) {
    // Scale uniformly.
    for (const auto& point : points_) {
      Point<N> scaled_point;
      for (std::size_t i = 0; i < N; ++i) {
        scaled_point[i] = point[i] * scale_size / max_length;
      }
      scaled_path.addPoint(scaled_point);
    }
  } else {
    // Scale non-uniformly.
    for (const auto& point : points_) {
      Point<N> scaled_point;
      for (std::size_t i = 0; i < N; ++i) {
        scaled_point[i] = point[i] * scale_size / (max[i] - min[i]);
      }
      scaled_path.addPoint(scaled_point);
    }
  }
  return scaled_path;
}

template <int N>
Path<N> Path<N>::translateCentroidTo(const Point<N>& p) const {
  Path<N> translated_path;
  Point<N> c = centroid();
  for (const auto& point : points_) {
    Point<N> translated_point;
    for (std::size_t i = 0; i < N; ++i) {
      translated_point[i] = point[i] + p[i] - c[i];
    }
    translated_path.addPoint(translated_point);
  }
  return translated_path;
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
