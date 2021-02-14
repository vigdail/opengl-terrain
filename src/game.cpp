#include "game.h"
#include "application.h"

#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "gui/gui_skybox.h"
#include "gui/gui_sun.h"

const uint Game::kKeysCount_;

Game::Game(uint width, uint height)
    : width_(width),
      height_(height),
      keys_(),
      camera_(Camera(glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f))),
      light_(DirectionalLight(glm::vec3(0.0f), glm::vec3(0.0f))),
      projection_(glm::perspective(glm::radians(60.0f), 1.0f * width_ / height_,
                                   0.1f, 1000.0f)),
      mouse_last_x_(0.0),
      mouse_last_y_(0.0) {
  LoadAssets();
  terrain_ = std::make_unique<Terrain>(100, 1024, 1024);
  skybox_ = std::make_unique<Skybox>();
  water_ = std::make_unique<WaterRenderer>(width_, height_);
  quad_ = std::make_unique<Quad>();

  gui_ = std::make_unique<GUILayer>(width, height);
  gui_->AddPanel(new GUISkyboxPanel(skybox_->GetAtmosphere()));
  gui_->AddPanel(new GUISunPanel(&light_));
}

void Game::LoadAssets() {
  ResourceManager::LoadShader("terrain", "../assets/shaders/terrain.vs",
                              "../assets/shaders/terrain.fs");
  ResourceManager::LoadShader("skybox", "../assets/shaders/skybox.vs",
                              "../assets/shaders/skybox.fs");
  ResourceManager::LoadShader("solid", "../assets/shaders/solid_color.vs",
                              "../assets/shaders/solid_color.fs");
  ResourceManager::LoadShader("sprite", "../assets/shaders/sprite.vs",
                              "../assets/shaders/sprite.fs");
  ResourceManager::LoadShader("water", "../assets/shaders/water.vs",
                              "../assets/shaders/water.fs");

  ResourceManager::LoadComputeShader(
      "compute_normalmap", "../assets/shaders/compute/normalmap.comp");
  ResourceManager::LoadComputeShader(
      "compute_heightmap", "../assets/shaders/compute/heightmap.comp");

  ResourceManager::LoadTexture("water_dudv",
                               "../assets/textures/water_dudv.png");
  ResourceManager::LoadTexture("water_normal",
                               "../assets/textures/water_normalmap.png");
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
}

void Game::Update(float dt) { gui_->Update(dt); }

void Game::Render() {
  // Water refraction pass
  water_->BindRefractionFramebuffer();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_CLIP_DISTANCE0);
  RenderScene(glm::vec4(0.0f, -1.0f, 0.0f, water_->GetHeight()));

  // Water reflection pass
  water_->BindReflectionFramebuffer();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  float dy = 2.0f * (camera_.position.y - water_->GetHeight());
  camera_.position.y -= dy;
  camera_.InvertPitch();
  RenderScene(glm::vec4(0.0f, 1.0f, 0.0f, -water_->GetHeight()));
  camera_.InvertPitch();
  camera_.position.y += dy;

  // Main pass
  glDisable(GL_CLIP_DISTANCE0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, width_, height_);
  RenderScene(glm::vec4(0.0f));

  water_->Render(&camera_, &light_, projection_);

  gui_->Render();
}

void Game::RenderScene(glm::vec4 clip_plane) {
  Shader &terrainShader = ResourceManager::GetShader("terrain");
  Shader &skyboxShader = ResourceManager::GetShader("skybox");

  terrainShader.Use();
  terrainShader.SetMat4("view", camera_.getViewMatrix());
  terrainShader.SetMat4("projection", projection_);
  terrainShader.SetVec3("light.direction",
                        glm::normalize(light_.GetDirection()));
  terrainShader.SetVec3("light.color", light_.GetColor());
  terrainShader.SetFloat("light.intensity", light_.GetIntensity());
  terrainShader.SetVec4("clipPlane", clip_plane);
  terrain_->Draw(terrainShader);

  glDepthFunc(GL_LEQUAL);
  glFrontFace(GL_CW);
  skyboxShader.Use();
  skyboxShader.SetMat4("view", glm::mat4(glm::mat3(camera_.getViewMatrix())));
  skyboxShader.SetMat4("projection", projection_);
  skyboxShader.SetVec3("camera", camera_.position);
  skyboxShader.SetVec3("sun.direction", glm::normalize(light_.GetDirection()));
  skyboxShader.SetVec3("sun.color", light_.GetColor());
  skyboxShader.SetFloat("sun.intensity", light_.GetIntensity());
  skybox_->Draw(skyboxShader);
  glFrontFace(GL_CCW);
  glDepthFunc(GL_LESS);
}

void Game::OnKeyEvent(int key, int scancode, int action, int mode) {
  gui_->OnKeyEvent(key, scancode, action, mode);

  if (action == GLFW_PRESS) {
    SetKeyPressed(key);
  } else if (action == GLFW_RELEASE) {
    SetKeyReleased(key);
  }
  if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
    camera_.Toggle();
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

void Game::SetKeyPressed(uint key) {
  if (key < kKeysCount_) {
    keys_[key] = true;
  }
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
