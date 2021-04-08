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

//float Terrain::getHeight(float x, float z) const {
//  glm::vec2 pos(x, z);
//
//  pos = pos * glm::vec2(res_x_, res_z_) / static_cast<float>(size_) + glm::vec2(res_x_ - 1, res_z_ - 1) / 2.0f;
//  int x0 = std::floor(pos.x);
//  int x1 = x0 + 1;
//  int z0 = std::floor(pos.y);
//  int z1 = z0 + 1;
//
//  float h0 = heights_[res_x_ * z0 + x0];
//  float h1 = heights_[res_x_ * z0 + x1];
//  float h2 = heights_[res_x_ * z1 + x0];
//  float h3 = heights_[res_x_ * z1 + x1];
//
//  float percent_u = pos.x - (float)x0;
//  float percent_v = pos.y - (float)z0;
//
//  float dU, dV;
//  if (percent_u > percent_v) {
//    dU = h1 - h0;
//    dV = h3 - h1;
//  } else {
//    dU = h3 - h2;
//    dV = h2 - h0;
//  }
//
//  float h = h0 + (dU * percent_u) + (dV * percent_v);
//  h *= scale_y_;
//
//  return h;
//}

Mesh Terrain::createMesh() {
  std::vector<Terrain::Vertex> vertices(16);
  uint32_t size = 4;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      float x = (float)j / (float)(size - 1);
      float z = (float)i / (float)(size - 1);
      Vertex v = {
          glm::vec2(x, z),
          glm::vec2(x, z),// @TODO: It seems redundant to store UVs
      };
      vertices[i * size + j] = v;
    }
  }

  Mesh mesh(PrimitiveTopology::PATCHES, vertices.size());
  mesh.addVertexBuffer(VertexBuffer{vertices, Terrain::Vertex::getLayout()});

  return mesh;
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
