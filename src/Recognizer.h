#pragma once

#include <map>
#include <vector>
#include "Path.h"

namespace NDRecognizer {
template <int N, class TemplateKeyT>
class Recognizer {
 public:
  explicit Recognizer(int sample_size, int scale_size);

  void addTemplate(const TemplateKeyT& name, const Path<N>& path);
  TemplateKeyT recognizeMinDistance(const Path<N>& path, float& score);
  TemplateKeyT recognizeMinAvgDistance(const Path<N>& path, float& score);

 private:
  Path<N> resampleScaleTranslate(const Path<N>& path);

  int sample_size_, scale_size_;
  std::map<TemplateKeyT, std::vector<Path<N>>> templates_;
};

template <int N, class TemplateKeyT>
Recognizer<N, TemplateKeyT>::Recognizer(int sample_size, int scale_size)
    : sample_size_(sample_size), scale_size_(scale_size) {}

template <int N, class TemplateKeyT>
void Recognizer<N, TemplateKeyT>::addTemplate(const TemplateKeyT& key,
                                              const Path<N>& path) {
  if (templates_.find(key) == templates_.end()) {
    templates_[key] = std::vector<Path<N>>();
  }
  templates_[key].push_back(resampleScaleTranslate(path));
}

template <int N, class TemplateKeyT>
TemplateKeyT Recognizer<N, TemplateKeyT>::recognizeMinDistance(
    const Path<N>& path, float& score) {
  Path<N> resampled_scaled_translated = resampleScaleTranslate(path);
  float min_distance = -1;
  TemplateKeyT matching_template;
  for (const auto& t : templates_) {
    for (const auto& path : t.second) {
      float distance = Distance(path, resampled_scaled_translated);
      if (distance < min_distance || min_distance == -1) {
        min_distance = distance;
        matching_template = t.first;
      }
    }
  }
  score =
      1 - (min_distance / (0.5 * std::sqrt(2 * (scale_size_ * scale_size_))));
  return matching_template;
}

template <int N, class TemplateKeyT>
TemplateKeyT Recognizer<N, TemplateKeyT>::recognizeMinAvgDistance(
    const Path<N>& path, float& score) {
  Path<N> resampled_scaled_translated = resampleScaleTranslate(path);
  float min_avg_distance = -1;
  TemplateKeyT matching_template;
  for (const auto& t : templates_) {
    float avg_distance = 0;
    for (const auto& path : t.second) {
      avg_distance += Distance(path, resampled_scaled_translated);
    }
    avg_distance /= t.second.size();
    if (avg_distance < min_avg_distance || min_avg_distance == -1) {
      min_avg_distance = avg_distance;
      matching_template = t.first;
    }
  }
  score = 1 - (min_avg_distance /
               (0.5 * std::sqrt(2 * (scale_size_ * scale_size_))));
  return matching_template;
}

template <int N, class TemplateKeyT>
Path<N> Recognizer<N, TemplateKeyT>::resampleScaleTranslate(
    const Path<N>& path) {
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
