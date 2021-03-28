#include "scene.h"
#include "application.h"

#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "gui/gui_skybox.h"
#include "gui/gui_sun.h"
#include "gui/gui_water.h"
#include "gui/gui_terrain.h"

const uint Scene::kKeysCount_;

Scene::Scene(uint width, uint height)
    : width_(width),
      height_(height),
      keys_(),
      camera_(Camera(60.0f, 1.0f * width_ / height_, 0.1f, 1000.0f)),
      light_(DirectionalLight(glm::vec3(0.0f), glm::vec3(0.0f))),
      mouse_last_x_(0.0),
      mouse_last_y_(0.0) {
  LoadAssets();
  camera_.position = glm::vec3(-200.0f, 3.0f, 0.0f);
  terrain_ = std::make_shared<Terrain>(1000, 1024, 1024);
  skybox_ = std::make_unique<Skybox>();
  water_ = std::make_shared<WaterRenderer>(width_, height_);
  quad_ = std::make_unique<Quad>();

  gui_ = std::make_unique<GUILayer>(width, height);
  gui_->AddPanel(new GUISkyboxPanel(skybox_->GetAtmosphere()));
  gui_->AddPanel(new GUISunPanel(&light_));
  gui_->AddPanel(new GUIWaterPanel(water_));
  gui_->AddPanel(new GUITerrainPanel(terrain_));
}

void Scene::LoadAssets() {
  ResourceManager::AddShader(
      "terrain",
      ShaderBuilder()
          .Load("../assets/shaders/terrain.vs", ShaderModule::Type::Vertex)
          .Load("../assets/shaders/terrain.fs", ShaderModule::Type::Fragment));
  ResourceManager::AddShader(
      "skybox",
      ShaderBuilder()
          .Load("../assets/shaders/skybox.vs", ShaderModule::Type::Vertex)
          .Load("../assets/shaders/skybox.fs", ShaderModule::Type::Fragment));
  ResourceManager::AddShader(
      "solid",
      ShaderBuilder()
          .Load("../assets/shaders/solid_color.vs", ShaderModule::Type::Vertex)
          .Load("../assets/shaders/solid_color.fs",
                ShaderModule::Type::Fragment));
  ResourceManager::AddShader(
      "sprite",
      ShaderBuilder()
          .Load("../assets/shaders/sprite.vs", ShaderModule::Type::Vertex)
          .Load("../assets/shaders/sprite.fs", ShaderModule::Type::Fragment));
  ResourceManager::AddShader(
      "water",
      ShaderBuilder()
          .Load("../assets/shaders/water.vs", ShaderModule::Type::Vertex)
          .Load("../assets/shaders/water.fs", ShaderModule::Type::Fragment));

  ResourceManager::AddShader(
      "compute_normalmap",
      ShaderBuilder().Load("../assets/shaders/compute/normalmap.comp",
                           ShaderModule::Type::Compute));
  ResourceManager::AddShader(
      "compute_heightmap",
      ShaderBuilder().Load("../assets/shaders/compute/heightmap.comp",
                           ShaderModule::Type::Compute));

  ResourceManager::AddTexture(
      "water_dudv", TextureBuilder().Load("../assets/textures/water_dudv.png"));
  ResourceManager::AddTexture(
      "water_normal",
      TextureBuilder().Load("../assets/textures/water_normalmap.png"));
}

void Scene::ProcessInput(float dt) {
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

void Scene::Update(float dt) {
  camera_.position.y =
      terrain_->GetHeight(camera_.position.x, camera_.position.z) + 1.7f;
  gui_->Update(dt);
}

void Scene::Render() {
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
  RenderScene(glm::vec4(0.0f, 1.0f, 0.0f, -water_->GetHeight() + 0.07f));
  camera_.InvertPitch();
  camera_.position.y += dy;

  // Main pass
  glDisable(GL_CLIP_DISTANCE0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, width_, height_);
  RenderScene(glm::vec4(0.0f));

  water_->Render(&camera_, &light_);

  gui_->Render();
}

void Scene::RenderScene(glm::vec4 clip_plane) {
  ShaderHandle terrainShader = ResourceManager::GetShader("terrain");
  ShaderHandle skyboxShader = ResourceManager::GetShader("skybox");

  terrainShader->Use();
  terrainShader->SetMat4("view", camera_.getViewMatrix());
  terrainShader->SetMat4("projection", camera_.getProjectionMatrix());
  terrainShader->SetVec3("light.direction",
                         glm::normalize(light_.GetDirection()));
  terrainShader->SetVec3("light.color", light_.GetColor());
  terrainShader->SetFloat("light.intensity", light_.GetIntensity());
  terrainShader->SetVec4("clipPlane", clip_plane);
  terrain_->Draw(terrainShader);

  glDepthFunc(GL_LEQUAL);
  glFrontFace(GL_CW);
  skyboxShader->Use();
  skyboxShader->SetMat4("view", glm::mat4(glm::mat3(camera_.getViewMatrix())));
  skyboxShader->SetMat4("projection", camera_.getProjectionMatrix());
  skyboxShader->SetVec3("camera", camera_.position);
  skyboxShader->SetVec3("sun.direction", glm::normalize(light_.GetDirection()));
  skyboxShader->SetVec3("sun.color", light_.GetColor());
  skyboxShader->SetFloat("sun.intensity", light_.GetIntensity());
  skybox_->Draw(skyboxShader);
  glFrontFace(GL_CCW);
  glDepthFunc(GL_LESS);
}

void Scene::OnKeyEvent(int key, int scancode, int action, int mode) {
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

void Scene::OnMouseButtonEvent(int button, int action, int mode) {
  gui_->OnMouseButtonEvent(button, action, mode);
}

void Scene::OnMousePositionEvent(double x, double y) {
  float offsetX = x - mouse_last_x_;
  float offsetY = mouse_last_y_ - y;

  mouse_last_x_ = x;
  mouse_last_y_ = y;

  camera_.handleMouseMovement(offsetX, offsetY);

  gui_->OnMousePositionEvent(x, y);
}

void Scene::SetKeyPressed(uint key) {
  if (key < kKeysCount_) {
    keys_[key] = true;
  }
}

void Scene::SetKeyReleased(uint key) {
  if (key < kKeysCount_) {
    keys_[key] = false;
  }
}

bool Scene::IsKeyPressed(uint key) {
  if (key < kKeysCount_) {
    return keys_[key];
  }

  return false;
}
