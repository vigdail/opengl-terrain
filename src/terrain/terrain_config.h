#pragma once

#include <array>
#include <glm/glm.hpp>

struct TerrainConfig {
  uint32_t root_nodes_count = 8;
  glm::ivec2 resolution = {512, 512};
  glm::vec3 scale = {3000.0f, 100.0f, 3000.0f};
  std::array<float, 8> lod_ranges = {
      1750,
      874,
      386,
      192,
      100,
      50,
      0,
      0,
  };
  std::array<int, 8> lod_morphing_areas{};
  TerrainConfig() {
    for (auto i = 0; i < 8; i++) {
      lod_morphing_areas[i] = lod_ranges[i] - updateMorphingArea(i + 1);
    }
  }

 private:
  float updateMorphingArea(int lod) const {
    return (int)((scale.x / root_nodes_count) / (std::pow(2, lod)));
  }
};
