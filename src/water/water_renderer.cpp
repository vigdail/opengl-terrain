#include "water_renderer.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

WaterRenderer::WaterRenderer(int width, int height)
    : mesh_(Plane(500.0f).toMesh()), height_(15.0f) {
  shader_ = ResourceManager::getShader("water");

  material_.specular_power = 32.0f;
  material_.reflection_power = 0.5f;
  material_.dudv_tiling = 6.0f;

  FrameBuffer::Spec spec;
  spec.width = width;
  spec.height = height;
  spec.color_formats = {GL_RGB};
  spec.depth_format = GL_DEPTH_COMPONENT24;

  refraction_framebuffer_ = std::make_unique<FrameBuffer>(spec);

  spec.width = width / 4;
  spec.height = height / 4;
  reflection_framebuffer_ = std::make_unique<FrameBuffer>(spec);

  dudv_map_ = ResourceManager::getTexture("water_dudv");
  normal_map_ = ResourceManager::getTexture("water_normal");
}

void WaterRenderer::bindReflectionFramebuffer() {
  reflection_framebuffer_->bind();
}

void WaterRenderer::bindRefractionFramebuffer() {
  refraction_framebuffer_->bind();
}

void WaterRenderer::render(Camera *camera, DirectionalLight *sun) {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  shader_->use();

  shader_->setInt("reflection", 0);
  shader_->setInt("refraction", 1);
  shader_->setInt("dudv", 2);
  shader_->setInt("normalmap", 3);
  shader_->setInt("depthmap", 4);

  reflection_framebuffer_->getTexture(0)->bind(0);
  refraction_framebuffer_->getTexture(0)->bind(1);
  dudv_map_->bind(2);
  normal_map_->bind(3);
  refraction_framebuffer_->getDepth()->bind(4);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, height_, 0.0f));
  model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

  shader_->setMat4("model", model);
  shader_->setMat4("view", camera->getViewMatrix());
  shader_->setMat4("projection", camera->getProjectionMatrix());
  shader_->setFloat("time", glfwGetTime());
  shader_->setVec3("camera_position", camera->position);
  shader_->setFloat("camera_near", camera->near);
  shader_->setFloat("camera_far", camera->far);
  shader_->setVec3("sun.direction", sun->getDirection());
  shader_->setVec3("sun.color", sun->getColor());
  shader_->setFloat("sun.intensity", sun->getIntensity());
  shader_->setFloat("reflection_power", material_.reflection_power);
  shader_->setFloat("specular_power", material_.specular_power);
  shader_->setFloat("dudv_tiling", material_.dudv_tiling);

  mesh_.bind();
  glDrawElements(static_cast<GLenum>(mesh_.getTopology()), mesh_.count(),
                 GL_UNSIGNED_INT, 0);

  glDisable(GL_BLEND);
}
