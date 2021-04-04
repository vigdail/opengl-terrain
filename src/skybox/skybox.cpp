#include "skybox.h"

#include <glad/glad.h>
#include <stb_image.h>

Skybox::Skybox() : mesh_(Sphere(64, 64, 1.0).ToMesh()) {}

Atmosphere& Skybox::GetAtmosphere() { return atmosphere_; }
Mesh& Skybox::GetMesh() { return mesh_; }
