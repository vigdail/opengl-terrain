#pragma once

#include "shader.h"
#include "sphere.h"

#include <glm/glm.hpp>

#include <string>
#include <vector>

struct Atmosphere {
  float planet_radius;
  float atmosphere_radius;
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
        g(0.82),
        beta_R(3.8e-6f, 13.5e-6f, 33.1e-6f),
        beta_M(21e-6f),
        view_dir_samples(16),
        sun_dir_samples(8) {}
};

class Skybox {
 public:
  Skybox();
  void Draw(Shader& shader);
  Atmosphere& GetAtmosphere();

 private:
  Sphere mesh_;
  Atmosphere atmosphere_;
};
