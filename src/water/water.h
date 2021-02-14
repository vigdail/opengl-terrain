#pragma once

#include <glm/glm.hpp>

#include "../shader.h"

class Water {
  struct Vertex {
    glm::vec3 position;
    glm::vec2 uv;
  };

 public:
  Water();
  ~Water();
  void Draw();

 private:
  unsigned int VAO_;
  unsigned int VBO_;
  unsigned int EBO_;

 private:
  void BuildVAO();
};
