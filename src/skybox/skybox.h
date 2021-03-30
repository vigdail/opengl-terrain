#pragma once

#include "../shader.h"
#include "../mesh/sphere.h"
#include "../resource_manager.h"

#include <glm/glm.hpp>

#include <string>
#include <vector>

struct Atmosphere {
  int planet_radius;
  int atmosphere_radius;
  float hR;
  float hM;
  float g;
  glm::vec3 beta_R;
  float beta_M;
  int view_dir_samples;
  int sun_dir_samples;

  Atmosphere()
      : planet_radius(6360e3),
        atmosphere_radius(6420e3),
        hR(7794),
        hM(1200),
        g(0.98),
        beta_R(3.8e-6f, 13.5e-6f, 33.1e-6f),
        beta_M(21e-6f),
        view_dir_samples(16),
        sun_dir_samples(8) {}
};

class Skybox {
 public:
  Skybox();
  void Draw();
  Atmosphere& GetAtmosphere();

 private:
  ShaderHandle shader_;
  Mesh mesh_;
  Atmosphere atmosphere_;
};
