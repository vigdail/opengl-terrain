#include "terrain.h"
#include "heightmap_renderer.h"
#include "normalmap_renderer.h"
#include "terrain_config.h"

#include <glad/glad.h>

Terrain::Terrain(TerrainConfig config)
    : config_(config),
      heights_(config.resolution.x * config.resolution.y) {
  mesh_ = std::make_shared<Mesh>(createMesh());

  transform_.position.x = -config.scale.x / 2.0f;
  transform_.position.y = 0.0f;
  transform_.position.z = -config.scale.z / 2.0f;
  transform_.scale = config.scale;

  nodes_.reserve(config.root_nodes_count * config.root_nodes_count);
  for (auto i = 0; i < config.root_nodes_count; i++) {
    for (auto j = 0; j < config.root_nodes_count; j++) {
      glm::vec2 location((float)i / config.root_nodes_count, (float)j / config.root_nodes_count);
      glm::vec2 index(i, j);
      nodes_.emplace_back(TerrainNode(config_, 0, location, index));
    }
  }

  heightmap_ = HeightmapRenderer::render(config.resolution.x, config.resolution.y, 8);
  normalmap_ = NormalmapRenderer::render(heightmap_);

  heightmap_.bind();
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, heights_.data());
}

float Terrain::getHeight(float x, float z) const {
  glm::vec2 pos(x, z);

  int res_x = config_.resolution.x;
  int res_z = config_.resolution.y;
  pos = pos * glm::vec2(res_x, res_z) / static_cast<float>(config_.scale.x) + glm::vec2(res_x - 1, res_z - 1) / 2.0f;
  int x_0 = std::floor(pos.x);
  int x_1 = x_0 + 1;
  int z_0 = std::floor(pos.y);
  int z_1 = z_0 + 1;

  float h_0 = heights_[res_x * z_0 + x_0];
  float h_1 = heights_[res_x * z_0 + x_1];
  float h_2 = heights_[res_x * z_1 + x_0];
  float h_3 = heights_[res_x * z_1 + x_1];

  float percent_u = pos.x - (float)x_0;
  float percent_v = pos.y - (float)z_0;

  float du, dv;
  if (percent_u > percent_v) {
    du = h_1 - h_0;
    dv = h_3 - h_1;
  } else {
    du = h_3 - h_2;
    dv = h_2 - h_0;
  }

  float h = h_0 + (du * percent_u) + (dv * percent_v);
  h *= config_.scale.y;

  return h;
}

void Terrain::update(const Camera &camera) {
  for (auto &node : nodes_) {
    node.update(camera);
  }
}

const std::shared_ptr<Mesh> &Terrain::getMesh() const {
  return mesh_;
}
const Texture &Terrain::getHeightmap() const {
  return heightmap_;
}
const Texture &Terrain::getNormalmap() const {
  return normalmap_;
}
const std::vector<TerrainNode> &Terrain::getNodes() const {
  return nodes_;
}
const TerrainConfig &Terrain::getConfig() const {
  return config_;
}
const Transform &Terrain::getTransform() const {
  return transform_;
}

Mesh Terrain::createMesh() {
  std::vector<Terrain::Vertex> vertices(16);
  uint32_t size = 4;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      float x = (float)j / (float)(size - 1);
      float z = (float)i / (float)(size - 1);
      Vertex v{glm::vec2(x, z)};
      vertices[i * size + j] = v;
    }
  }

  Mesh mesh(PrimitiveTopology::PATCHES, vertices.size());
  mesh.addVertexBuffer(VertexBuffer{vertices, Terrain::Vertex::getLayout()});

  return mesh;
}
