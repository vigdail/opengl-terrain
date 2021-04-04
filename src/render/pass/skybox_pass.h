#pragma once

#include "../render_context.h"
#include "../../scene.h"
#include "../../resource_manager.h"

class SkyboxPass {
 public:
  void Render(Scene *scene, RenderContext *context) {
    glDepthFunc(GL_LEQUAL);
    glFrontFace(GL_CW);

    shader_ = ResourceManager::GetShader("skybox");
    Skybox *skybox = scene->skybox.get();
    Atmosphere &atmosphere = skybox->GetAtmosphere();

    shader_->Use();
    shader_->SetMat4("view",
                     glm::mat4(glm::mat3(scene->camera.getViewMatrix())));
    shader_->SetMat4("projection", scene->camera.getProjectionMatrix());
    shader_->SetVec3("camera", scene->camera.position);
    shader_->SetVec3("sun.direction",
                     glm::normalize(scene->light.GetDirection()));
    shader_->SetVec3("sun.color", scene->light.GetColor());
    shader_->SetFloat("sun.intensity", scene->light.GetIntensity());

    shader_->SetFloat("planet_radius", atmosphere.planet_radius);
    shader_->SetFloat("atmosphere_radius", atmosphere.atmosphere_radius);
    shader_->SetFloat("hR", atmosphere.hR);
    shader_->SetFloat("hM", atmosphere.hM);
    shader_->SetFloat("g", atmosphere.g);
    shader_->SetVec3("beta_R", atmosphere.beta_R);
    shader_->SetFloat("beta_M", atmosphere.beta_M);
    shader_->SetInt("view_dir_samples", atmosphere.view_dir_samples);
    shader_->SetInt("sun_dir_samples", atmosphere.sun_dir_samples);

    skybox->GetMesh().Bind();
    glDrawElements(static_cast<GLenum>(topology_), skybox->GetMesh().Count(),
                   GL_UNSIGNED_INT, nullptr);

    glDepthFunc(GL_LESS);
    glFrontFace(GL_CCW);
  }

 private:
  ShaderHandle shader_ = nullptr;
  PrimitiveTopology topology_ = PrimitiveTopology::Triangle;
};
