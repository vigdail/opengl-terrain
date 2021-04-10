#pragma once

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

struct Transform {
  glm::vec3 position{0.0f};
  glm::vec3 scale{1.0f};

  glm::mat4 getMatrix() const {
    glm::mat4 model(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);

    return model;
  }
};