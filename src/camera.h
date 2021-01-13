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
const float SPEED = 5.0f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;

class Camera {
 public:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;

  glm::vec3 worldUp;

  float yaw;
  float pitch;

  float speed;
  float sensitivity;
  float fov;

  Camera();
  Camera(glm::vec3 position, glm::vec3 up, float yaw = YAW,
         float pitch = PITCH);

  glm::mat4 getViewMatrix();

  void move(CameraMovement direction, float dt);
  void handleMouseMovement(float dx, float dy, bool contrainPitch = true);
  void handleMouseScroll(float dy);

 private:
  void updateVectors();
};
