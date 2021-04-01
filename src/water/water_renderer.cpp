#include "water_renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

WaterRenderer::WaterRenderer(int width, int height)
    : mesh_(Plane(500.0f).ToMesh()), height_(15.0f) {
  shader_ = ResourceManager::GetShader("water");

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

  dudv_map_ = ResourceManager::GetTexture("water_dudv");
  normal_map_ = ResourceManager::GetTexture("water_normal");
}

void WaterRenderer::BindReflectionFramebuffer() {
  reflection_framebuffer_->Bind();
}

void WaterRenderer::BindRefractionFramebuffer() {
  refraction_framebuffer_->Bind();
}

void WaterRenderer::Render(Camera *camera, DirectionalLight *sun) {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  shader_->Use();

  shader_->SetInt("reflection", 0);
  shader_->SetInt("refraction", 1);
  shader_->SetInt("dudv", 2);
  shader_->SetInt("normalmap", 3);
  shader_->SetInt("depthmap", 4);

  reflection_framebuffer_->GetTexture(0)->Bind(0);
  refraction_framebuffer_->GetTexture(0)->Bind(1);
  dudv_map_->Bind(2);
  normal_map_->Bind(3);
  refraction_framebuffer_->GetDepth()->Bind(4);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, height_, 0.0f));
  model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

  shader_->SetMat4("model", model);
  shader_->SetMat4("view", camera->getViewMatrix());
  shader_->SetMat4("projection", camera->getProjectionMatrix());
  shader_->SetFloat("time", glfwGetTime());
  shader_->SetVec3("camera_position", camera->position);
  shader_->SetFloat("camera_near", camera->near);
  shader_->SetFloat("camera_far", camera->far);
  shader_->SetVec3("sun.direction", sun->GetDirection());
  shader_->SetVec3("sun.color", sun->GetColor());
  shader_->SetFloat("sun.intensity", sun->GetIntensity());
  shader_->SetFloat("reflection_power", material_.reflection_power);
  shader_->SetFloat("specular_power", material_.specular_power);
  shader_->SetFloat("dudv_tiling", material_.dudv_tiling);

  mesh_.Bind();
  glDrawElements(static_cast<GLenum>(mesh_.GetTopology()), mesh_.Count(),
                 GL_UNSIGNED_INT, 0);

  glDisable(GL_BLEND);
}
