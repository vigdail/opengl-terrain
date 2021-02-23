#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <memory>

#include "shader.h"
#include "texture.h"

class Terrain {
  struct Vertex {
    glm::vec2 position;
    glm::vec2 uv;
  };

 public:
  Terrain();
  explicit Terrain(int size);
  Terrain(int size, int res_x, int res_z);
  void Draw(Shader &shader);
  float GetHeight(float x, float z) const;
  float GetScaleY() { return scale_y_; }
  void SetScaleY(float scale_y) { scale_y_ = scale_y; }

 private:
  int res_x_;
  int res_z_;
  int size_;
  float scale_y_;
  std::vector<Vertex> vertices_;
  std::vector<int> indices_;
  std::vector<float> heights_;
  std::size_t indices_count_;
  // @TODO: Make some wrappers maybe
  unsigned int VAO_;
  unsigned int EBO_;
  unsigned int VBO_;
  Texture heightmap_;
  Texture normalmap_;

 private:
  void GenerateVertices();
  void GenerateIndices();
  void BuildVAO();
  int GetIndex(int x, int z) { return z * res_x_ + x; }
};
