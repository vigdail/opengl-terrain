#include "skybox.h"

Skybox::Skybox() : mesh_(Sphere(64, 64, 1.0).toMesh()) {}

Atmosphere &Skybox::getAtmosphere() { return atmosphere_; }
Mesh &Skybox::getMesh() { return mesh_; }
