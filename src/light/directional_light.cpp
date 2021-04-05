
#include "directional_light.h"

DirectionalLight::DirectionalLight(glm::vec3 position, glm::vec3 look_at,
                                   float intensity)
    : intensity_(intensity),
      position_(position),
      look_at_(look_at),
      direction_(glm::normalize(position - look_at)),
      color_(glm::vec3(1.0f)) {}

glm::vec3 DirectionalLight::getDirection() {
  return glm::normalize(direction_);
}
void DirectionalLight::setDirection(glm::vec3 direction) {
  direction_ = direction;
}

glm::vec3 DirectionalLight::getPosition() { return position_; }
void DirectionalLight::setPosition(glm::vec3 position) {
  position_ = position;
  direction_ = glm::normalize(position_ - look_at_);
}
glm::vec3 DirectionalLight::getColor() { return color_; }
float DirectionalLight::getIntensity() const { return intensity_; }
void DirectionalLight::setIntensity(float intensity) { intensity_ = intensity; }
