#pragma once

#include "pass/skybox_pass.h"
#include "render_context.h"
#include "../scene.h"

class Renderer {
 public:
  void Render(Scene *scene) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    skybox_pass_.Render(scene, &context_);
  }

 private:
  RenderContext context_;
  SkyboxPass skybox_pass_;
  // Atmosphere pass
  // Terrain pass
  // Skybox pass
  // other objects pass
  // Water reflections pass
  // Water refractions pass
  // Water pass
  // HDR postprocessing
};
