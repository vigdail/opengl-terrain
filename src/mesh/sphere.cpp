#include "sphere.h"
#include <glad/glad.h>
#include <cmath>
#include <iostream>

// @TODO: Fill initial vectors size
Sphere::Sphere(int sectors, int stacks, float radius)
    : sectors_(sectors),
      stacks_(stacks),
      radius_(radius),
      vertices_(),
      indices_() {
  assert(sectors > 0);
  assert(stacks > 0);
  GenerateVertices();
  GenerateIndices();
}

void Sphere::GenerateVertices() {
  float sector_step = 2 * M_PI / sectors_;
  float stack_step = M_PI / stacks_;

  for (int i = 0; i <= stacks_; ++i) {
    float stack_angle = i * stack_step;
    float xz = sinf(stack_angle);
    float y = cosf(stack_angle);

    for (int j = 0; j <= sectors_; ++j) {
      float sector_angle = j * sector_step;

      float x = xz * cosf(sector_angle);
      float z = xz * sinf(sector_angle);
      glm::vec3 position = glm::vec3(x, y, z) * radius_;

      glm::vec3 normal(x, y, z);
      float s = static_cast<float>(j) / sectors_;
      float t = static_cast<float>(i) / stacks_;
      glm::vec2 uv(s, t);
      vertices_.push_back({position, normal, uv});
    }
  }
}

void Sphere::GenerateIndices() {
  for (int i = 0; i < stacks_; ++i) {
    int k1 = i * (sectors_ + 1);
    int k2 = k1 + sectors_ + 1;

    for (int j = 0; j < sectors_; ++j, ++k1, ++k2) {
      if (i != 0) {
        indices_.push_back(k1);
        indices_.push_back(k1 + 1);
        indices_.push_back(k2);
      }

      if (i != (stacks_ - 1)) {
        indices_.push_back(k1 + 1);
        indices_.push_back(k2 + 1);
        indices_.push_back(k2);
      }
    }
  }
}
