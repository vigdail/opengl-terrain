#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "../resource_manager.h"
#include "../shader.h"
#include "../texture.h"

class Terrain {
  struct Vertex {
    glm::vec2 position;
    glm::vec2 uv;
  };

 public:
  Terrain();
  explicit Terrain(int size);
  Terrain(int size, int res_x, int res_z);
  void draw(ShaderHandle shader);
  float getHeight(float x, float z) const;
  float getScaleY() const { return scale_y_; }
  void setScaleY(float scale_y) { scale_y_ = scale_y; }

 private:
  int res_x_;
  int res_z_;
  int size_;
  float scale_y_;
  std::vector<Vertex> vertices_;
  std::vector<int> indices_;
  std::vector<float> heights_;
  std::size_t indices_count_{};
  // @TODO: Make some wrappers maybe
  uint32_t vao_{};
  uint32_t ebo_{};
  uint32_t vbo_{};
  Texture heightmap_;
  Texture normalmap_;

 private:
  void generateVertices();
  void generateIndices();
  void buildVao();
  int getIndex(int x, int z) const { return z * res_x_ + x; }
};
