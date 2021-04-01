#pragma once

#include <memory>

#include "../mesh/shape/plane.h"
#include "../resource_manager.h"
#include "../camera.h"
#include "../shader.h"
#include "../frame_buffer.h"
#include "../light/directional_light.h"

struct WaterMaterial {
  float specular_power;
  float reflection_power;
  float dudv_tiling;
};

class WaterRenderer {
 public:
  WaterRenderer(int width, int height);
  void Render(Camera *camera, DirectionalLight *sun);
  void BindReflectionFramebuffer();
  void BindRefractionFramebuffer();
  float GetHeight() const { return height_; }
  void SetHeight(float height) { height_ = height; }
  WaterMaterial &GetMaterial() { return material_; }

 private:
  ShaderHandle shader_;
  Mesh mesh_;
  TextureHandle dudv_map_;
  TextureHandle normal_map_;
  std::unique_ptr<FrameBuffer> reflection_framebuffer_;
  std::unique_ptr<FrameBuffer> refraction_framebuffer_;
  float height_;
  WaterMaterial material_;
};
