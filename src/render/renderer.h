#pragma once

#include "../scene.h"
#include "pass/skybox_pass.h"
#include "pass/terrain_pass.h"
#include "pass/water_pass.h"
#include "render_context.h"

struct CameraUBO {
  glm::vec3 camera_position;
  alignas(16) glm::mat4 view;
  glm::mat4 projection;
};

class Renderer {
 public:
  Renderer(uint32_t width, uint32_t height) {
    camera_ubo_ = std::make_unique<UniformBuffer>(sizeof(glm::mat4) * 2 + sizeof(glm::vec4));
    createFramebuffers(width, height);
    skybox_pass_ = std::make_unique<SkyboxPass>();
    terrain_pass_ = std::make_unique<TerrainPass>();
    water_pass_ = std::make_unique<WaterPass>(reflection_framebuffer_, refraction_framebuffer_);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    camera_ubo_->bindBase(0);
  }

  void render(Scene *scene) {
    // update UBOs
    updateCameraUbo(scene->camera);

    // Water refraction
    refraction_framebuffer_->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CLIP_DISTANCE0);
    context_.setClipPlane(glm::vec4(0.0f, -1.0f, 0.0f, scene->water->getHeight() + 2.0f));
    terrain_pass_->render(scene, &context_);
    skybox_pass_->render(scene, &context_);

    // Water reflection
    reflection_framebuffer_->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float dy = 2.0f * (scene->camera.position.y - scene->water->getHeight());
    scene->camera.position.y -= dy;
    scene->camera.invertPitch();
    updateCameraUbo(scene->camera);
    context_.setClipPlane(glm::vec4(0.0f, 1.0f, 0.0f, -scene->water->getHeight() + 0.1f));
    terrain_pass_->render(scene, &context_);
    skybox_pass_->render(scene, &context_);
    scene->camera.invertPitch();
    scene->camera.position.y += dy;
    updateCameraUbo(scene->camera);

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
  std::unique_ptr<UniformBuffer> camera_ubo_;
  // @TODO: Compute Atmosphere pass
  // @TODO: other objects pass
  // @TODO: Reflection and refraction passes with subpasses
  // @TODO: HDR postprocessing

  void updateCameraUbo(Camera &camera) const {
    CameraUBO ubo{};
    ubo.camera_position = camera.position;
    ubo.view = camera.getViewMatrix();
    ubo.projection = camera.getProjectionMatrix();
    camera_ubo_->setData(&ubo, 0, sizeof(ubo));
  }

  void createFramebuffers(uint32_t width, uint32_t height) {
    FrameBuffer::Spec spec;
    spec.width = width;
    spec.height = height;
    spec.color_formats = {GL_RGB};
    spec.depth_format = GL_DEPTH_COMPONENT24;

    refraction_framebuffer_ = std::make_unique<FrameBuffer>(spec);

    spec.width = width / 2;
    spec.height = height / 2;
    reflection_framebuffer_ = std::make_unique<FrameBuffer>(spec);
  }
};
