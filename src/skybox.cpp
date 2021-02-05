#include "skybox.h"

#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>

Skybox::Skybox() : mesh_(128, 128, 1.0), atmosphere_() {}

void Skybox::Draw(Shader &shader) {
  shader.Use();
  mesh_.Draw();
}

Atmosphere &Skybox::GetAtmosphere() { return atmosphere_; }
