#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "shader.h"
#include "utils/heightmap_generator.h"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;
};

class Terrain {
 public:
  Terrain();
  explicit Terrain(int size);
  Terrain(int size, int res_x, int res_z);
  void Draw(Shader& shader);

 private:
  int res_x_;
  int res_z_;
  int size_;
  std::vector<Vertex> vertices_;
  std::vector<int> indices_;
  // @TODO: Make some wrappers maybe
  unsigned int VAO_;
  unsigned int EBO_;
  unsigned int VBO_;
  HeightmapGenerator generator_;

  void GenerateVertices();
  void GenerateIndices();
  void GenerateNormals();
  void BuildVAO();
  int GetIndex(int x, int z);
  void SetHeightmap(const std::vector<float>& heightmap);
};
