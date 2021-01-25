#include "skybox.h"

#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>

Skybox::Skybox() : mesh_(8, 8, 1.0) {}

void Skybox::Draw(Shader &shader) { mesh_.Draw(shader); }
