#pragma once

#include "../scene.h"
#include "pass/skybox_pass.h"
#include "pass/terrain_pass.h"
#include "render_context.h"

class Renderer {
 public:
  void render(Scene *scene) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    skybox_pass_.render(scene, &context_);
    terrain_pass_.render(scene, &context_);
    scene->gui->render();
  }

 private:
  RenderContext context_;
  SkyboxPass skybox_pass_;
  TerrainPass terrain_pass_;
  // Atmosphere pass
  // other objects pass
  // Water reflections pass
  // Water refractions pass
  // Water pass
  // HDR postprocessing
};
