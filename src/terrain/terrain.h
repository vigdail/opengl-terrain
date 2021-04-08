#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "../mesh/mesh.h"
#include "../resource_manager.h"
#include "../shader.h"
#include "../texture.h"
#include "terrain_config.h"
#include "terrain_node.h"

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
  explicit Terrain(TerrainConfig config);
  //  float getHeight(float x, float z) const;
  float getScaleY() const { return config_.scale.y; }
  void setScaleY(float scale_y) { config_.scale.y = scale_y; }
  const Texture &getHeightmap() const;
  const Texture &getNormalmap() const;
  const std::shared_ptr<Mesh> &getMesh() const;
  const std::vector<TerrainNode> &getNodes() const;
  void update(const Camera &camera) {
    for (auto &node : nodes_) {
      node.update(camera);
    }
  }

 private:
  TerrainConfig config_{};

 public:
  const TerrainConfig &getConfig() const;

 private:
  std::vector<float> heights_;
  std::vector<TerrainNode> nodes_;

 private:
  std::shared_ptr<Mesh> mesh_;
  Transform transform_{};

 public:
  const Transform &getTransform() const;

 private:
  Texture heightmap_;
  Texture normalmap_;

  //  int getIndex(int x, int z) const { return z * res_x_ + x; }
  static Mesh createMesh();
};
