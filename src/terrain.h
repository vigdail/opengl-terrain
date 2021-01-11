#pragma once

#include <vector>
#include <glm/glm.hpp>

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;
};

class Terrain {
 public:
  Terrain();
  Terrain(int width, int length);

 private:
  int width_;
  int length_;
  std::vector<Vertex> vertices_;
  std::vector<int> indices_;
};
