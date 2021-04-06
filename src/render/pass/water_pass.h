#pragma once

#include "../../scene.h"
#include "../render_context.h"

class WaterPass {
 public:
  WaterPass(std::shared_ptr<FrameBuffer> reflection_framebuffer, std::shared_ptr<FrameBuffer> refraction_framebuffer);
  void render(Scene *scene, RenderContext *context);

 private:
  std::shared_ptr<FrameBuffer> reflection_framebuffer_;
  std::shared_ptr<FrameBuffer> refraction_framebuffer_;
  ShaderHandle shader_ = nullptr;
  PrimitiveTopology topology_ = PrimitiveTopology::TRIANGLE;
};
