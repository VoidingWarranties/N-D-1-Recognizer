#pragma once

#include <string>
#include <map>
#include "Path.h"

namespace NDRecognizer {
template <int N>
class Recognizer {
 public:
  explicit Recognizer(int sample_size, int scale_size);

  void addTemplate(const std::string& name, const Path<N>& path);
  std::string recognize(const Path<N>& path, float& score);

 private:
  Path<N> resampleScaleTranslate(const Path<N>& path);

  int sample_size_, scale_size_;
  std::map<std::string, Path<N>> templates_;
};

template <int N>
Recognizer<N>::Recognizer(int sample_size, int scale_size)
    : sample_size_(sample_size), scale_size_(scale_size) {}

template <int N>
void Recognizer<N>::addTemplate(const std::string& name, const Path<N>& path) {
  templates_[name] = resampleScaleTranslate(path);
}

template <int N>
std::string Recognizer<N>::recognize(const Path<N>& path, float& score) {
  Path<N> resampled_scaled_translated = resampleScaleTranslate(path);
  float min_distance = -1;
  std::string template_name = "";
  for (const auto& t : templates_) {
    float distance = Distance(t.second, resampled_scaled_translated);
    if (distance < min_distance || min_distance == -1) {
      min_distance = distance;
      template_name = t.first;
    }
  }
  score =
      1 - (min_distance / (0.5 * std::sqrt(2 * (scale_size_ * scale_size_))));
  return template_name;
}

template <int N>
Path<N> Recognizer<N>::resampleScaleTranslate(const Path<N>& path) {
  Path<N> resampled_path = path.resample(sample_size_);
  Path<N> scaled_path = resampled_path.scale(scale_size_);
  Point<N> origin;
  for (size_t i = 0; i < N; ++i) {
    origin[i] = 0;
  }
  Path<N> translated_path = scaled_path.translateCentroidTo(origin);
  return translated_path;
}
}
