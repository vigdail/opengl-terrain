#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() : Camera(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f) {}

Camera::Camera(float fov, float aspect, float near, float far)
    : position(glm::vec3(0.0f)),
      front(glm::vec3(0.0f, 0.0f, -1.0f)),
      worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
      yaw(YAW),
      pitch(PITCH),
      speed(SPEED),
      sensitivity(SENSITIVITY),
      fov(fov),
      near(near),
      far(far),
      active_(true),
      projection_(glm::perspective(glm::radians(fov), aspect, near, far)) {
  updateVectors();
}

glm::mat4 Camera::getViewMatrix() {
  return glm::lookAt(position, position + front, up);
}

void Camera::Enable() { active_ = true; }
void Camera::Disable() { active_ = false; }
void Camera::Toggle() { active_ = !active_; }

void Camera::InvertPitch() {
  pitch = -pitch;
  updateVectors();
}

void Camera::move(CameraMovement direction, float dt) {
  if (!active_) {
    return;
  }
  glm::vec3 dir = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
  switch (direction) {
    case CameraMovement::FORWARD:
      position += dir * speed * dt;
      break;
    case CameraMovement::BACKWARD:
      position -= dir * speed * dt;
      break;
    case CameraMovement::LEFT:
      position -= right * speed * dt;
      break;
    case CameraMovement::RIGHT:
      position += right * speed * dt;
      break;
    default:
      break;
  }
}
void Camera::handleMouseMovement(float dx, float dy, bool contrainPitch) {
  if (!active_) {
    return;
  }
  const float sensetivity = 0.1f;
  dx *= sensetivity;
  dy *= sensetivity;

  yaw += dx;
  pitch += dy;

  if (contrainPitch) {
    if (pitch > 89.0f) {
      pitch = 89.0f;
    }
    if (pitch < -89.0f) {
      pitch = -89.0f;
    }
  }

  updateVectors();
}

void Camera::updateVectors() {
  glm::vec3 _front;
  _front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  _front.y = sin(glm::radians(pitch));
  _front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(_front);

  right = glm::normalize(glm::cross(front, worldUp));
  up = glm::normalize(glm::cross(right, front));
}
