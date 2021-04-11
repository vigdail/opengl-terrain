#pragma once

#include "../camera.h"
#include "../components/components.h"
#include "../mesh/mesh.h"
#include "terrain_config.h"
#include <memory>

class TerrainNode {
 public:
  TerrainNode(const std::shared_ptr<TerrainConfig> &config, int lod, const glm::vec2 &location, const glm::vec2 &index);
  bool isLeaf() const;
  void update(const Camera &camera);
  const Transform &getTransform() const;
  const std::vector<TerrainNode> &getNodes() const;
  int getLod() const;
  const glm::vec2 &getLocation() const;
  const glm::vec2 &getIndex() const;
  float getGap() const;

 private:
  std::shared_ptr<TerrainConfig> config_;
  Transform transform_{};
  int lod_;
  glm::vec2 location_;
  glm::vec2 index_;
  bool is_leaf_ = true;

 private:
  float gap_;
  std::vector<TerrainNode> nodes_;
  glm::vec3 world_position_{};

 private:
  void updateChildNodes(const Camera &camera);
  void addChildNodes(int lod);
  void removeChildNodes();
};
