#pragma once

#include "../scene.h"
#include "pass/skybox_pass.h"
#include "pass/terrain_pass.h"
#include "pass/water_pass.h"
#include "render_context.h"

class Renderer {
 public:
  Renderer(uint32_t width, uint32_t height) {
    createFramebuffers(width, height);
    skybox_pass_ = std::make_unique<SkyboxPass>();
    terrain_pass_ = std::make_unique<TerrainPass>();
    water_pass_ = std::make_unique<WaterPass>(reflection_framebuffer_, refraction_framebuffer_);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
  }

  void render(Scene *scene) {
    // Water refraction
    refraction_framebuffer_->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CLIP_DISTANCE0);
    context_.setClipPlane(glm::vec4(0.0f, -1.0f, 0.0f, scene->water->getHeight()));
    terrain_pass_->render(scene, &context_);
    skybox_pass_->render(scene, &context_);

    // Water reflection
    reflection_framebuffer_->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float dy = 2.0f * (scene->camera.position.y - scene->water->getHeight());
    scene->camera.position.y -= dy;
    scene->camera.invertPitch();
    context_.setClipPlane(glm::vec4(0.0f, 1.0f, 0.0f, -scene->water->getHeight() + 0.07f));
    terrain_pass_->render(scene, &context_);
    skybox_pass_->render(scene, &context_);
    scene->camera.invertPitch();
    scene->camera.position.y += dy;

    // Main pass
    glDisable(GL_CLIP_DISTANCE0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, scene->getWidth(), scene->getHeight());
    context_.setClipPlane(glm::vec4(0.0f));
    terrain_pass_->render(scene, &context_);
    skybox_pass_->render(scene, &context_);
    water_pass_->render(scene, &context_);
    scene->gui->render();
  }

 private:
  RenderContext context_;
  std::shared_ptr<FrameBuffer> reflection_framebuffer_;
  std::shared_ptr<FrameBuffer> refraction_framebuffer_;
  std::unique_ptr<SkyboxPass> skybox_pass_;
  std::unique_ptr<TerrainPass> terrain_pass_;
  std::unique_ptr<WaterPass> water_pass_;
  // @TODO: Compute Atmosphere pass
  // @TODO: other objects pass
  // @TODO: Reflection and refraction passes with subpasses
  // @TODO: HDR postprocessing

  void createFramebuffers(uint32_t width, uint32_t height) {
    FrameBuffer::Spec spec;
    spec.width = width;
    spec.height = height;
    spec.color_formats = {GL_RGB};
    spec.depth_format = GL_DEPTH_COMPONENT24;

    refraction_framebuffer_ = std::make_unique<FrameBuffer>(spec);

    spec.width = width / 4;
    spec.height = height / 4;
    reflection_framebuffer_ = std::make_unique<FrameBuffer>(spec);
  }
};
