#pragma once

#include <glm/glm.hpp>

class DirectionalLight {
 public:
  DirectionalLight(glm::vec3 position, glm::vec3 look_at, float intensity = 20);
  glm::vec3 getDirection();
  glm::vec3 getPosition();
  void setPosition(glm::vec3 position);
  glm::vec3 getColor();
  float getIntensity() const;
  void setIntensity(float intensity);
  void setDirection(glm::vec3 direction);

 private:
  float intensity_;
  glm::vec3 position_;
  glm::vec3 look_at_;
  glm::vec3 direction_;
  glm::vec3 color_;
};
