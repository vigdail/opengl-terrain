#include "terrain_node.h"
#include "terrain_config.h"

#include <utility>

TerrainNode::TerrainNode(const TerrainConfig &config, int lod, const glm::vec2 &location, const glm::vec2 &index)
    : config_(config), lod_(lod), location_(location), index_(index) {
  gap_ = 1.0f / (config.root_nodes_count * (float)(std::pow(2, lod)));

  glm::vec3 local_scale(gap_, 0, gap_);
  glm::vec3 local_position(location.x, 0, location.y);
  transform_.position = local_position;
  transform_.scale = local_scale;

  glm::vec2 scale_xz = glm::vec2(config.scale.x, config.scale.z);
  glm::vec2 loc = (location + gap_ / 2.0f) * scale_xz - scale_xz / 2.0f;
  //  float height = getTerrainHeight(loc.getX(), loc.getY());
  world_position_ = glm::vec3(loc.x, 0, loc.y);
}

bool TerrainNode::isLeaf() const {
  return is_leaf_;
}

void TerrainNode::update(const Camera &camera) {
  updateChildNodes(camera);
  for (auto &node : nodes_) {
    node.update(camera);
  }
}

void TerrainNode::updateChildNodes(const Camera &camera) {
  float distance = glm::distance(camera.position, world_position_);
  if (distance < config_.lod_ranges[lod_]) {
    addChildNodes(lod_ + 1);
  } else if (distance >= config_.lod_ranges[lod_]) {
    removeChildNodes();
  }
}
void TerrainNode::addChildNodes(int lod) {
  if (is_leaf_) {
    is_leaf_ = false;
  }

  if (nodes_.empty()) {
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
        glm::vec2 location = location_ + glm::vec2(i * gap_ / 2.0f, j * gap_ / 2.0f);
        TerrainNode node(config_, lod, location, glm::vec2(i, j));
        nodes_.emplace_back(std::move(node));
      }
    }
  }
}

void TerrainNode::removeChildNodes() {
  if (!is_leaf_) {
    is_leaf_ = true;
  }
  if (!nodes_.empty()) {
    nodes_.clear();
  }
}
const std::vector<TerrainNode> &TerrainNode::getNodes() const {
  return nodes_;
}
const Transform &TerrainNode::getTransform() const {
  return transform_;
}
int TerrainNode::getLod() const {
  return lod_;
}
const glm::vec2 &TerrainNode::getLocation() const {
  return location_;
}
const glm::vec2 &TerrainNode::getIndex() const {
  return index_;
}
float TerrainNode::getGap() const {
  return gap_;
}
