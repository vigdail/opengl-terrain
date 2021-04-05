#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() : Camera(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f) {}

Camera::Camera(float fov, float aspect, float near, float far)
    : position(glm::vec3(0.0f)), front(glm::vec3(0.0f, 0.0f, -1.0f)),
      world_up(glm::vec3(0.0f, 1.0f, 0.0f)), yaw(YAW), pitch(PITCH),
      speed(SPEED), sensitivity(SENSITIVITY), fov(fov), near(near), far(far),
      active_(true),
      projection_(glm::perspective(glm::radians(fov), aspect, near, far)) {
  updateVectors();
}

glm::mat4 Camera::getViewMatrix() const {
  return glm::lookAt(position, position + front, up);
}

void Camera::enable() { active_ = true; }
void Camera::disable() { active_ = false; }
void Camera::toggle() { active_ = !active_; }

void Camera::invertPitch() {
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
void Camera::handleMouseMovement(float dx, float dy, bool constrain_pitch) {
  if (!active_) {
    return;
  }
  dx *= sensitivity;
  dy *= sensitivity;

  yaw += dx;
  pitch += dy;

  if (constrain_pitch) {
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
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(front);

  right = glm::normalize(glm::cross(front, world_up));
  up = glm::normalize(glm::cross(right, front));
}

void Camera::handleMouseScroll(float dy) {}
