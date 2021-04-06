#pragma once

#include "../../scene.h"
#include "../render_context.h"

class TerrainPass {
 public:
  TerrainPass() = default;
  void render(Scene *scene, RenderContext *context);

 private:
  ShaderHandle shader_ = nullptr;
  PrimitiveTopology topology_ = PrimitiveTopology::TRIANGLE;
};
