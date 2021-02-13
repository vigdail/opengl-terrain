#include "water_renderer.h"
#include "../resource_manager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

WaterRenderer::WaterRenderer() : height_(1.7f) {
  water_shader_ = &ResourceManager::GetShader("water");
  water_ = std::make_unique<Water>();

  // @TODO: set sizes
  FrameBuffer::Spec spec;
  spec.width = 1024;
  spec.height = 1024;
  spec.color_formats = {GL_RGB};
  spec.depth_format = GL_DEPTH_COMPONENT24;

  refraction_framebuffer_ = std::make_unique<FrameBuffer>(spec);

  spec.width = 256;
  spec.height = 256;
  reflection_framebuffer_ = std::make_unique<FrameBuffer>(spec);

  dudv_map_ = &ResourceManager::GetTexture("water-dudv");

  dudv_map_->Bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void WaterRenderer::BindReflectionFramebuffer() {
  reflection_framebuffer_->Bind();
}

void WaterRenderer::BindRefractionFramebuffer() {
  refraction_framebuffer_->Bind();
}

void WaterRenderer::Render(Camera *camera, glm::mat4 projection) {
  water_shader_->Use();
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, height_, 0.0f));
  model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
  glActiveTexture(GL_TEXTURE0);
  BindReflectionTexture();
  glActiveTexture(GL_TEXTURE1);
  BindRefractionTexture();
  glActiveTexture(GL_TEXTURE2);
  dudv_map_->Bind();
  water_shader_->SetMat4("model", model);
  water_shader_->SetMat4("view", camera->getViewMatrix());
  water_shader_->SetMat4("projection", projection);
  water_shader_->SetInt("reflection", 0);
  water_shader_->SetInt("refraction", 1);
  water_shader_->SetInt("dudv", 2);
  water_shader_->SetFloat("time", glfwGetTime());

  water_->Draw();
}
