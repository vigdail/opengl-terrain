#include "heightmap_generator.h"

#include <glm/gtc/noise.hpp>
#include <algorithm>

HeightmapGenerator::HeightmapGenerator() : HeightmapGenerator(512, 512, 10) {}
HeightmapGenerator::HeightmapGenerator(int width, int height)
    : HeightmapGenerator(width, height, 8) {}

HeightmapGenerator::HeightmapGenerator(int width, int height, int octaves)
    : width_(width),
      height_(height),
      octaves_(octaves),
      data_(std::vector<float>(width * height)) {}

void HeightmapGenerator::Generate() {
  for (int j = 0; j < height_; j++) {
    for (int i = 0; i < width_; i++) {
      float freq = 3.0f;
      float amp = 1.0f;
      float norm = 0.0f;
      float value = 0.0f;
      for (int o = 0; o < octaves_; o++) {
        float x = static_cast<float>(i) / width_ * freq;
        float y = static_cast<float>(j) / height_ * freq;
        value += amp * glm::perlin(glm::vec2(x, y));
        freq *= 2.0f;
        norm += amp;
        amp /= 2.0f;
      }
      data_[j * width_ + i] = value / norm;
    }
  }
}

Texture HeightmapGenerator::CreateTexture() {
  Texture texture;
  texture.image_format = GL_RED;
  texture.internal_format = GL_RED;
  // texture.filter_mag = GL_NEAREST;
  // texture.filter_min = GL_NEAREST;
  std::vector<unsigned char> image_data(width_ * height_);
  std::transform(data_.begin(), data_.end(), image_data.begin(),
                 [](float value) {
                   return static_cast<unsigned char>(0.5 * (value + 1) * 255.0);
                 });

  texture.Generate(width_, height_, image_data.data());

  return texture;
}

const std::vector<float> &HeightmapGenerator::GetData() { return data_; }
