
#include "directional_light.h"

DirectionalLight::DirectionalLight(glm::vec3 position, glm::vec3 look_at,
                                   float intensity)
    : intensity_(intensity),
      position_(position),
      look_at_(look_at),
      direction_(glm::normalize(position - look_at)),
      color_(glm::vec3(1.0f)) {}

glm::vec3 DirectionalLight::GetDirection() {
  return glm::normalize(direction_);
}
void DirectionalLight::SetDirection(glm::vec3 direction) {
  direction_ = direction;
}

glm::vec3 DirectionalLight::GetPosition() { return position_; }
void DirectionalLight::SetPosition(glm::vec3 position) {
  position_ = position;
  direction_ = glm::normalize(position_ - look_at_);
}
glm::vec3 DirectionalLight::GetColor() { return color_; }
float DirectionalLight::GetIntensity() { return intensity_; }
void DirectionalLight::SetIntensity(float intensity) { intensity_ = intensity; }
