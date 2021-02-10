#pragma once

#include <memory>

#include "water.h"
#include "../camera.h"
#include "../shader.h"
#include "../frame_buffer.h"

class WaterRenderer {
 public:
  WaterRenderer();
  void Render(Camera *camera, glm::mat4 projection);
  void BindReflectionFramebuffer();
  void BindRefractionFramebuffer();

 private:
  Shader *water_shader_;
  std::unique_ptr<Water> water_;
  std::unique_ptr<FrameBuffer> reflection_framebuffer_;
  std::unique_ptr<FrameBuffer> refraction_framebuffer_;
};
