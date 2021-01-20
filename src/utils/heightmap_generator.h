#pragma once

#include <vector>

#include "../texture.h"

class HeightmapGenerator {
 public:
  HeightmapGenerator();
  HeightmapGenerator(int width, int height);
  HeightmapGenerator(int width, int height, int octaves);
  void Generate();
  const std::vector<float> &GetData();
  Texture CreateTexture();

 private:
  int width_;
  int height_;
  int octaves_;
  std::vector<float> data_;
};
