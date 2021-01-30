#pragma once

#include "shader.h"
#include "sphere.h"

#include <string>
#include <vector>

class Skybox {
 public:
  Skybox();
  void Draw(Shader& shader);

 private:
  Sphere mesh_;
};
