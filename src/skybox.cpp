#include "skybox.h"

#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>

Skybox::Skybox() : mesh_(128, 128, 1.0), atmosphere_() {}

void Skybox::Draw(Shader &shader) {
  shader.Use();
  shader.SetFloat("planet_radius", atmosphere_.planet_radius);
  shader.SetFloat("atmosphere_radius", atmosphere_.atmosphere_radius);
  shader.SetFloat("hR", atmosphere_.hR);
  shader.SetFloat("hM", atmosphere_.hM);
  shader.SetFloat("g", atmosphere_.g);
  shader.SetVec3("beta_R", atmosphere_.beta_R);
  shader.SetFloat("beta_M", atmosphere_.beta_M);
  shader.SetInt("view_dir_samples", atmosphere_.view_dir_samples);
  shader.SetInt("sun_dir_samples", atmosphere_.sun_dir_samples);
  mesh_.Draw();
}

Atmosphere &Skybox::GetAtmosphere() { return atmosphere_; }
