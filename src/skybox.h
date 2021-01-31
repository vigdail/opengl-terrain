#pragma once

#include "shader.h"
#include "sphere.h"

#include <glm/glm.hpp>

#include <string>
#include <vector>

struct Atmosphere {
  glm::vec3 color;
  float Br;
  float Bm;
  float g;

  Atmosphere()
      : color(0.650, 0.570, 0.475), Br(0.0025), Bm(0.0003), g(0.9800) {}
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
