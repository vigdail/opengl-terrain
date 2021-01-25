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
  BuildVAO();
}

void Sphere::Draw(Shader &shader) {
  shader.Use();
  shader.SetVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glBindVertexArray(VAO_);
  glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  // glPointSize(4);
  // glDrawArrays(GL_POINTS, 0, vertices_.size());
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

void Sphere::BuildVAO() {
  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);
  glGenBuffers(1, &EBO_);

  glBindVertexArray(VAO_);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex),
               vertices_.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int),
               indices_.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        static_cast<void *>(0));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void *>(offsetof(Vertex, normal)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void *>(offsetof(Vertex, uv)));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
