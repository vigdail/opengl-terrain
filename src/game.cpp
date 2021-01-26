#include "game.h"
#include "application.h"

#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const uint Game::kKeysCount_;

Game::Game(uint width, uint height)
    : width_(width),
      height_(height),
      keys_(),
      camera_(Camera(glm::vec3(0.0f, 1.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f))),
      light_(DirectionalLight(glm::vec3(10.0f, 5.0f, 0.0), glm::vec3(0.0f))),
      mouse_last_x_(0.0),
      mouse_last_y_(0.0) {
  LoadAssets();
  terrain_ = std::make_unique<Terrain>(100, 1024, 1024);
  gui_ = std::make_unique<GUILayer>(width, height);
  skybox_ = std::make_unique<Skybox>();
}

void Game::LoadAssets() {
  ResourceManager::LoadShader("terrain", "../assets/shaders/terrain.vs",
                              "../assets/shaders/terrain.fs");
  ResourceManager::LoadShader("skybox", "../assets/shaders/skybox.vs",
                              "../assets/shaders/skybox.fs");
  ResourceManager::LoadShader("solid", "../assets/shaders/solid_color.vs",
                              "../assets/shaders/solid_color.fs");

  ResourceManager::LoadComputeShader(
      "compute_normalmap", "../assets/shaders/compute/normalmap.comp");
  ResourceManager::LoadComputeShader(
      "compute_heightmap", "../assets/shaders/compute/heightmap.comp");
}

void Game::ProcessInput(float dt) {
  if (keys_[GLFW_KEY_W]) {
    camera_.move(CameraMovement::FORWARD, dt);
  }
  if (keys_[GLFW_KEY_S]) {
    camera_.move(CameraMovement::BACKWARD, dt);
  }
  if (keys_[GLFW_KEY_A]) {
    camera_.move(CameraMovement::LEFT, dt);
  }
  if (keys_[GLFW_KEY_D]) {
    camera_.move(CameraMovement::RIGHT, dt);
  }

  if (keys_[GLFW_KEY_SPACE]) {
    camera_.Toggle();
  }
}

void Game::Update(float dt) {
  light_.SetPosition(glm::vec3(1.0f * cos(glfwGetTime() / 5.0f),
                               1.0f * sin(glfwGetTime() / 5.0f), 0.0f));
  gui_->Update(dt);
}

void Game::Render() {
  glm::mat4 projection =
      glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 1000.0f);

  Shader &terrainShader = ResourceManager::GetShader("terrain");
  terrainShader.Use();
  terrainShader.SetMat4("view", camera_.getViewMatrix());
  terrainShader.SetMat4("projection", projection);
  terrainShader.SetVec3("light.direction", light_.GetDirection());
  terrainShader.SetVec3("light.color", light_.GetColor());
  terrain_->Draw(terrainShader);

  glDepthFunc(GL_LEQUAL);
  glFrontFace(GL_CW);
  Shader &skyboxShader = ResourceManager::GetShader("skybox");
  skyboxShader.Use();
  skyboxShader.SetMat4("view", glm::mat4(glm::mat3(camera_.getViewMatrix())));
  skyboxShader.SetMat4("projection", projection);
  skyboxShader.SetVec3("light.direction", light_.GetDirection());
  skyboxShader.SetVec3("light.color", light_.GetColor());
  skybox_->Draw(skyboxShader);
  glFrontFace(GL_CCW);
  glDepthFunc(GL_LESS);

  gui_->Render();
}

void Game::SetKeyPressed(uint key) {
  if (key < kKeysCount_) {
    keys_[key] = true;
  }
}

void Game::OnKeyEvent(int key, int scancode, int action, int mode) {
  gui_->OnKeyEvent(key, scancode, action, mode);

  if (action == GLFW_PRESS) {
    SetKeyPressed(key);
  } else if (action == GLFW_RELEASE) {
    SetKeyReleased(key);
  }
}

void Game::OnMouseButtonEvent(int button, int action, int mode) {
  gui_->OnMouseButtonEvent(button, action, mode);
}

void Game::OnMousePositionEvent(double x, double y) {
  float offsetX = x - mouse_last_x_;
  float offsetY = mouse_last_y_ - y;

  mouse_last_x_ = x;
  mouse_last_y_ = y;

  camera_.handleMouseMovement(offsetX, offsetY);

  gui_->OnMousePositionEvent(x, y);
}

void Game::SetKeyReleased(uint key) {
  if (key < kKeysCount_) {
    keys_[key] = false;
  }
}

bool Game::IsKeyPressed(uint key) {
  if (key < kKeysCount_) {
    return keys_[key];
  }

  return false;
}
