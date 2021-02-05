#pragma once

#include <glm/glm.hpp>

class DirectionalLight {
 public:
  DirectionalLight(glm::vec3 position, glm::vec3 look_at, float intensity = 20);
  glm::vec3 GetDirection();
  glm::vec3 GetPosition();
  void SetPosition(glm::vec3 position);
  glm::vec3 GetColor();
  float GetIntensity();

 private:
  float intensity_;
  glm::vec3 position_;
  glm::vec3 look_at_;
  glm::vec3 color_;
};
