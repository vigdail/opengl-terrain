#pragma once

#include <memory>

#include "../camera.h"
#include "../frame_buffer.h"
#include "../light/directional_light.h"
#include "../mesh/shape/plane.h"
#include "../resource_manager.h"
#include "../shader.h"

struct WaterMaterial {
  float specular_power = 32.0f;
  float reflection_power = 0.5f;
  float dudv_tiling = 6.0f;
  float wave_strength = 0.003f;
  glm::vec3 color = {0.0f, 0.4f, 0.6f};
  float depth_factor = 2.0f;
  float wave_speed_factor = 250.0f;
  TextureHandle dudv_map;
  TextureHandle normal_map;
};

class Water {
 public:
  Water();
  float getHeight() const { return height_; }
  void setHeight(float height) { height_ = height; }
  WaterMaterial &getMaterial() { return material_; }
  const Mesh &getMesh() const;

 private:
  Mesh mesh_;
  float height_;
  WaterMaterial material_{};
};
