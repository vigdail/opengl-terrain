#include "water.h"

Water::Water()
    : mesh_(Plane(500.0f).toMesh()), height_(15.0f) {
  material_.dudv_map = ResourceManager::getTexture("water_dudv");
  material_.normal_map = ResourceManager::getTexture("water_normal");
}
const Mesh &Water::getMesh() const {
  return mesh_;
}