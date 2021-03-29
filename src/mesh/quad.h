#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "mesh.h"

// @TODO: Use this as water mesh maybe? (code is the same for now)
class Quad {
  struct Vertex {
    glm::vec3 position;
    glm::vec2 uv;
  };

 public:
  Quad() {
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);

    BuildVAO();
  }

  void Draw() {
    glBindVertexArray(VAO_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }

 private:
  uint32_t VAO_;
  uint32_t VBO_;
  uint32_t EBO_;

 private:
  void BuildVAO() {
    std::array<Vertex, 4> vertices_ = {
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f),
    };
    std::array<unsigned int, 6> indices_ = {0, 1, 2, 2, 1, 3};

    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex),
                 vertices_.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices_.size() * sizeof(unsigned int), indices_.data(),
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          static_cast<void*>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, uv)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }
};
