
#include "directional_light.h"

DirectionalLight::DirectionalLight(glm::vec3 position, glm::vec3 look_at,
                                   float intensity)
    : intensity_(intensity),
      position_(position),
      look_at_(look_at),
      color_(glm::vec3(1.0f)) {}

glm::vec3 DirectionalLight::GetDirection() {
  return glm::normalize(position_ - look_at_);
}
glm::vec3 DirectionalLight::GetPosition() { return position_; }
void DirectionalLight::SetPosition(glm::vec3 position) { position_ = position; }
glm::vec3 DirectionalLight::GetColor() { return color_; }
float DirectionalLight::GetIntensity() { return intensity_; }
