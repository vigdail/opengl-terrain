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

    static BufferLayout getLayout() {
      VertexAttribute position_attr{};
      position_attr.count = 2;
      position_attr.offset = 0;
      position_attr.location = 0;
      position_attr.normalized = false;

      BufferLayout layout{};
      layout.stride = sizeof(Vertex);
      layout.attributes = {position_attr};

      return layout;
    }
  };

 public:
  explicit Terrain(TerrainConfig config);
  float getHeight(float x, float z) const;
  float getScaleY() const { return config_.scale.y; }
  void setScaleY(float scale_y) { config_.scale.y = scale_y; }
  const Texture &getHeightmap() const;
  const Texture &getNormalmap() const;
  const std::shared_ptr<Mesh> &getMesh() const;
  const std::vector<TerrainNode> &getNodes() const;
  const Transform &getTransform() const;
  const TerrainConfig &getConfig() const;
  void update(const Camera &camera);

 private:
  TerrainConfig config_{};
  std::vector<float> heights_;
  std::vector<TerrainNode> nodes_;
  std::shared_ptr<Mesh> mesh_;
  Transform transform_{};
  Texture heightmap_;
  Texture normalmap_;

  static Mesh createMesh();
};
