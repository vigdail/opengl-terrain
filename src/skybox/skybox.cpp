#include "skybox.h"

#include <glad/glad.h>
#include <stb_image.h>

Skybox::Skybox() : mesh_(Sphere(128, 128, 1.0).ToMesh()), atmosphere_() {
  shader_ = ResourceManager::GetShader("skybox");
}

void Skybox::Draw() {
  shader_->Use();
  shader_->SetFloat("planet_radius",
                    static_cast<float>(atmosphere_.planet_radius));
  shader_->SetFloat("atmosphere_radius",
                    static_cast<float>(atmosphere_.atmosphere_radius));
  shader_->SetFloat("hR", atmosphere_.hR);
  shader_->SetFloat("hM", atmosphere_.hM);
  shader_->SetFloat("g", atmosphere_.g);
  shader_->SetVec3("beta_R", atmosphere_.beta_R);
  shader_->SetFloat("beta_M", atmosphere_.beta_M);
  shader_->SetInt("view_dir_samples", atmosphere_.view_dir_samples);
  shader_->SetInt("sun_dir_samples", atmosphere_.sun_dir_samples);
  mesh_.Bind();
  glDrawElements(static_cast<GLenum>(mesh_.GetTopology()), mesh_.Count(),
                 GL_UNSIGNED_INT, 0);
}

Atmosphere &Skybox::GetAtmosphere() { return atmosphere_; }
