#include "water_renderer.h"
#include "../resource_manager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

WaterRenderer::WaterRenderer(int width, int height) : height_(2.0f) {
  water_shader_ = &ResourceManager::GetShader("water");
  water_ = std::make_unique<Water>();

  FrameBuffer::Spec spec;
  spec.width = width;
  spec.height = height;
  spec.color_formats = {GL_RGB};
  spec.depth_format = GL_DEPTH_COMPONENT24;

  refraction_framebuffer_ = std::make_unique<FrameBuffer>(spec);

  spec.width = width / 4;
  spec.height = height / 4;
  reflection_framebuffer_ = std::make_unique<FrameBuffer>(spec);

  dudv_map_ = &ResourceManager::GetTexture("water_dudv");
  dudv_map_->Bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  normal_map_ = &ResourceManager::GetTexture("water_normal");
  normal_map_->Bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void WaterRenderer::BindReflectionFramebuffer() {
  reflection_framebuffer_->Bind();
}

void WaterRenderer::BindRefractionFramebuffer() {
  refraction_framebuffer_->Bind();
}

void WaterRenderer::Render(Camera *camera, DirectionalLight *sun,
                           glm::mat4 projection) {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  water_shader_->Use();

  reflection_framebuffer_->GetTexture(0)->Bind(0);
  refraction_framebuffer_->GetTexture(0)->Bind(1);
  dudv_map_->Bind(2);
  normal_map_->Bind(3);
  refraction_framebuffer_->GetDepth()->Bind(4);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, height_, 0.0f));
  model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

  water_shader_->SetMat4("model", model);
  water_shader_->SetMat4("view", camera->getViewMatrix());
  water_shader_->SetMat4("projection", projection);
  water_shader_->SetInt("reflection", 0);
  water_shader_->SetInt("refraction", 1);
  water_shader_->SetInt("dudv", 2);
  water_shader_->SetInt("normalmap", 3);
  water_shader_->SetInt("depthmap", 4);
  water_shader_->SetFloat("time", glfwGetTime());
  water_shader_->SetVec3("camera_position", camera->position);
  water_shader_->SetVec3("sun.direction", glm::normalize(sun->GetDirection()));
  water_shader_->SetVec3("sun.color", sun->GetColor());
  water_shader_->SetFloat("sun.intensity", sun->GetIntensity());

  water_->Draw();

  glDisable(GL_BLEND);
}
