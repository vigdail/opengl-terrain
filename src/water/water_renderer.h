#pragma once

#include <memory>

#include "water.h"
#include "../camera.h"
#include "../shader.h"
#include "../frame_buffer.h"
#include "../light/directional_light.h"

class WaterRenderer {
 public:
  WaterRenderer(int width, int height);
  void Render(Camera *camera, DirectionalLight *sun, glm::mat4 projection);
  void BindReflectionFramebuffer();
  void BindRefractionFramebuffer();
  float GetHeight() { return height_; }

 private:
  Shader *water_shader_;
  std::unique_ptr<Water> water_;
  Texture *dudv_map_;
  Texture *normal_map_;
  std::unique_ptr<FrameBuffer> reflection_framebuffer_;
  std::unique_ptr<FrameBuffer> refraction_framebuffer_;
  float height_;
};
