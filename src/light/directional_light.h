#pragma once

#include <glm/glm.hpp>

class DirectionalLight {
 public:
  DirectionalLight(glm::vec3 position, glm::vec3 look_at);
  explicit DirectionalLight(glm::vec3 direction);
  glm::vec3 GetDirection();
  void SetPosition(glm::vec3 position);
  glm::vec3 GetColor();

 private:
  glm::vec3 position_;
  glm::vec3 look_at_;
  glm::vec3 color_;
};
