#pragma once

#include <glm/glm.hpp>

enum CameraMovement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
};

// Default values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 50.0f;
const float SENSITIVITY = 0.1f;

class Camera {
 public:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up{};
  glm::vec3 right{};

  glm::vec3 world_up;

  float yaw;
  float pitch;

  float speed;
  float sensitivity;
  float fov;
  float near;
  float far;

  Camera();
  Camera(float fov, float aspect, float near, float far);

  glm::mat4 getViewMatrix() const;
  glm::mat4 getProjectionMatrix() { return projection_; }

  void move(CameraMovement direction, float dt);
  void handleMouseMovement(float dx, float dy, bool constrain_pitch = true);
  void handleMouseScroll(float dy);
  void disable();
  void enable();
  void toggle();
  void invertPitch();

 private:
  bool active_;
  glm::mat4 projection_;
  void updateVectors();
};
