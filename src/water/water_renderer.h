#pragma once

#include <memory>

#include "water.h"
#include "../camera.h"
#include "../shader.h"

class WaterRenderer {
 public:
  WaterRenderer();
  void Render(Camera *camera, glm::mat4 projection);

 private:
  Shader *water_shader_;
  std::unique_ptr<Water> water_;
};
