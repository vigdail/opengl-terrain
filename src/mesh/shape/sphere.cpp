#include "sphere.h"
#include <cmath>

// @TODO: Fill initial vectors size
Sphere::Sphere(int sectors, int stacks, float radius)
    : sectors_(sectors),
      stacks_(stacks),
      radius_(radius),
      vertices_(),
      indices_() {
  assert(sectors > 0);
  assert(stacks > 0);
  generateVertices();
  generateIndices();
}

void Sphere::generateVertices() {
  float sector_step = M_PI / sectors_ * 2.0f;
  float stack_step = M_PI / stacks_;

  for (int i = 0; i <= stacks_; ++i) {
    float stack_angle = (float)i * stack_step;
    float xz = sinf(stack_angle);
    float y = cosf(stack_angle);

    for (int j = 0; j <= sectors_; ++j) {
      float sector_angle = (float)j * sector_step;

      float x = xz * cosf(sector_angle);
      float z = xz * sinf(sector_angle);
      glm::vec3 position = glm::vec3(x, y, z) * radius_;

      glm::vec3 normal(x, y, z);
      float s = (float)j / (float)sectors_;
      float t = (float)i / (float)stacks_;
      glm::vec2 uv(s, t);
      vertices_.push_back({position, normal, uv});
    }
  }
}

void Sphere::generateIndices() {
  for (int i = 0; i < stacks_; ++i) {
    int k = i * (sectors_ + 1);
    int m = k + sectors_ + 1;

    for (int j = 0; j < sectors_; ++j, ++k, ++m) {
      if (i != 0) {
        indices_.push_back(k);
        indices_.push_back(k + 1);
        indices_.push_back(m);
      }

      if (i != (stacks_ - 1)) {
        indices_.push_back(k + 1);
        indices_.push_back(m + 1);
        indices_.push_back(m);
      }
    }
  }
}
