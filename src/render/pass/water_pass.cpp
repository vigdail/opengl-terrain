#include "water_pass.h"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <utility>

WaterPass::WaterPass(std::shared_ptr<FrameBuffer> reflection_framebuffer, std::shared_ptr<FrameBuffer> refraction_framebuffer)
    : reflection_framebuffer_(std::move(reflection_framebuffer)),
      refraction_framebuffer_(std::move(refraction_framebuffer)) {}

void WaterPass::render(Scene *scene, RenderContext *context) {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Water *water = scene->water.get();
  WaterMaterial &material = water->getMaterial();

  shader_ = ResourceManager::getShader("water");
  shader_->use();

  shader_->setInt("reflection", 0);
  shader_->setInt("refraction", 1);
  shader_->setInt("dudv", 2);
  shader_->setInt("normalmap", 3);
  shader_->setInt("depthmap", 4);

  reflection_framebuffer_->getTexture(0)->bind(0);
  refraction_framebuffer_->getTexture(0)->bind(1);
  material.dudv_map->bind(2);
  material.normal_map->bind(3);
  refraction_framebuffer_->getDepth()->bind(4);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, water->getHeight(), 0.0f));
  model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

  shader_->setMat4("model", model);
  shader_->setMat4("view", scene->camera.getViewMatrix());
  shader_->setMat4("projection", scene->camera.getProjectionMatrix());
  shader_->setVec3("camera_position", scene->camera.position);
  shader_->setFloat("camera_near", scene->camera.near);
  shader_->setFloat("camera_far", scene->camera.far);
  shader_->setFloat("time", glfwGetTime());
  shader_->setVec3("sun.direction", scene->light.getDirection());
  shader_->setVec3("sun.color", scene->light.getColor());
  shader_->setFloat("sun.intensity", scene->light.getIntensity());
  shader_->setFloat("reflection_power", material.reflection_power);
  shader_->setFloat("specular_power", material.specular_power);
  shader_->setFloat("dudv_tiling", material.dudv_tiling);
  shader_->setVec3("color", material.color);
  shader_->setFloat("wave_strength", material.wave_strength);
  shader_->setFloat("depth_factor", material.depth_factor);
  shader_->setFloat("wave_speed", material.wave_speed_factor);

  water->getMesh().bind();
  glDrawElements(static_cast<GLenum>(topology_), water->getMesh().count(), GL_UNSIGNED_INT, nullptr);

  glDisable(GL_BLEND);
}
