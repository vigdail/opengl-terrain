#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "shader.h"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;
};

class Terrain {
 public:
  Terrain();
  Terrain(int width, int length);
  void Draw(Shader &shader);

 private:
  int width_;
  int length_;
  std::vector<Vertex> vertices_;
  std::vector<int> indices_;
  // @TODO: Make some wrappers maybe
  unsigned int VAO_;
  unsigned int EBO_;
  unsigned int VBO_;

  void GenerateVertices();
  void GenerateIndices();
  void BuildVAO();
};
