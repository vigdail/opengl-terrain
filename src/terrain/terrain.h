#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "../mesh/mesh.h"
#include "../resource_manager.h"
#include "../shader.h"
#include "../texture.h"

class Terrain {
  struct Vertex {
    glm::vec2 position;
    glm::vec2 uv;

    static BufferLayout getLayout() {
      VertexAttribute position_attr{};
      position_attr.count = 2;
      position_attr.offset = 0;
      position_attr.location = 0;
      position_attr.normalized = false;
      VertexAttribute uv_attr{};
      uv_attr.count = 2;
      uv_attr.offset = offsetof(Vertex, uv);
      uv_attr.location = 1;
      uv_attr.normalized = false;

      BufferLayout layout{};
      layout.stride = sizeof(Vertex);
      layout.attributes = {position_attr, uv_attr};

      return layout;
    }
  };

 public:
  Terrain();
  explicit Terrain(int size);
  Terrain(int size, int res_x, int res_z);
  float getHeight(float x, float z) const;
  float getScaleY() const { return scale_y_; }
  void setScaleY(float scale_y) { scale_y_ = scale_y; }
  const Texture &getHeightmap() const;
  const Texture &getNormalmap() const;
  const Mesh &getMesh() const;

 private:
  int res_x_;
  int res_z_;
  int size_;
  float scale_y_ = 100.0f;
  std::vector<float> heights_;
  Mesh mesh_;
  Texture heightmap_;
  Texture normalmap_;

 private:
  int getIndex(int x, int z) const { return z * res_x_ + x; }
  Mesh createMesh();
  std::vector<Vertex> generateVertices() const;
  std::vector<uint32_t> generateIndices() const;
};
