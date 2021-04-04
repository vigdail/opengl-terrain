#pragma once

#include "../shader.h"
#include "../mesh/shape/sphere.h"
#include "../resource_manager.h"

#include <glm/glm.hpp>

#include <string>
#include <vector>

struct Atmosphere {
  float planet_radius = 6360e3;
  float atmosphere_radius = 6420e3;
  float hR = 7794;
  float hM = 1200;
  float g = 0.98;
  glm::vec3 beta_R = {3.8e-6f, 13.5e-6f, 33.1e-6f};
  float beta_M = 21e-6f;
  int view_dir_samples = 16;
  int sun_dir_samples = 8;
};

class Skybox {
 public:
  Skybox();
  // void Draw();
  Atmosphere& GetAtmosphere();
  Mesh& GetMesh();

 private:
  // ShaderHandle shader_;
  Mesh mesh_;
  Atmosphere atmosphere_;
};
