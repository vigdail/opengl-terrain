#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "shader.h"

struct Vertex {
  glm::vec3 position;
  glm::vec2 uv;
};

class Terrain {
 public:
  Terrain();
  explicit Terrain(int size);
  Terrain(int size, int res_x, int res_z);
  void Draw(Shader &shader);

 private:
  int res_x_;
  int res_z_;
  int size_;
  std::vector<Vertex> vertices_;
  std::vector<glm::vec3> normals_;
  std::vector<int> indices_;
  // @TODO: Make some wrappers maybe
  unsigned int VAO_;
  unsigned int EBO_;
  unsigned int VBO_;

  void GenerateVertices();
  void GenerateIndices();
  void BuildVAO();
  float GetHeight(int x, int z);
};
