#include "scene.h"
#include "application.h"

#include <iostream>
#include <string>
#include <utility>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "gui/gui_skybox.h"
#include "gui/gui_sun.h"
#include "gui/gui_water.h"
#include "gui/gui_terrain.h"

const uint Scene::kKeysCount_;

Scene::Scene(uint width, uint height)
    : camera(Camera(60.0f, 1.0f * width / height, 0.1f, 1000.0f)),
      light(DirectionalLight(glm::vec3(0.0f), glm::vec3(0.0f))),
      keys_(),
      width_(width),
      height_(height),
      mouse_last_x_(0.0),
      mouse_last_y_(0.0) {
  LoadAssets();
  camera.position = glm::vec3(-440.0f, 3.0f, 0.0f);
  terrain_ = std::make_shared<Terrain>(1000, 1024, 1024);
  skybox = std::make_unique<Skybox>();
  water_ = std::make_shared<WaterRenderer>(width, height);

  auto quad = std::make_shared<Mesh>(Quad().ToMesh());
  meshes_.push_back(quad);

  gui_ = std::make_unique<GUILayer>(width, height);
  gui_->AddPanel(new GUISkyboxPanel(skybox->GetAtmosphere()));
  gui_->AddPanel(new GUISunPanel(&light));
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

  TextureSamplerDescriptor sampler{};
  sampler.wrap_s = GL_REPEAT;
  sampler.wrap_t = GL_REPEAT;
  ResourceManager::AddTexture("water_dudv",
                              TextureBuilder()
                                  .Load("../assets/textures/water_dudv.png")
                                  .WithSampler(sampler));
  ResourceManager::AddTexture(
      "water_normal", TextureBuilder()
                          .Load("../assets/textures/water_normalmap.png")
                          .WithSampler(sampler));
}

void Scene::ProcessInput(float dt) {
  if (keys_[GLFW_KEY_W]) {
    camera.move(CameraMovement::FORWARD, dt);
  }
  if (keys_[GLFW_KEY_S]) {
    camera.move(CameraMovement::BACKWARD, dt);
  }
  if (keys_[GLFW_KEY_A]) {
    camera.move(CameraMovement::LEFT, dt);
  }
  if (keys_[GLFW_KEY_D]) {
    camera.move(CameraMovement::RIGHT, dt);
  }
}

void Scene::Update(float dt) {
  camera.position.y =
      terrain_->GetHeight(camera.position.x, camera.position.z) + 1.7f;
  gui_->Update(dt);
}

// void Scene::Render() {
//   // Water refraction pass
//   water_->BindRefractionFramebuffer();
//   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//   glEnable(GL_CLIP_DISTANCE0);
//   RenderScene(glm::vec4(0.0f, -1.0f, 0.0f, water_->GetHeight()));

//   // Water reflection pass
//   water_->BindReflectionFramebuffer();
//   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//   float dy = 2.0f * (camera.position.y - water_->GetHeight());
//   camera.position.y -= dy;
//   camera.InvertPitch();
//   RenderScene(glm::vec4(0.0f, 1.0f, 0.0f, -water_->GetHeight() + 0.07f));
//   camera.InvertPitch();
//   camera.position.y += dy;

//   // Main pass
//   glDisable(GL_CLIP_DISTANCE0);
//   glBindFramebuffer(GL_FRAMEBUFFER, 0);
//   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//   glViewport(0, 0, width_, height_);
//   RenderScene(glm::vec4(0.0f));

//   water_->Render(&camera, &light);

//   ShaderHandle sprite = ResourceManager::GetShader("sprite");
//   sprite->Use();
//   glm::mat4 model(1.0f);
//   model = glm::translate(model, glm::vec3((width_ - 200.0f), 0.0f, 0.0f));
//   model = glm::scale(model, glm::vec3(200.0f, 200.0f, 1.0f));
//   glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(width_),
//                               static_cast<float>(height_), 0.0f);
//   sprite->SetMat4("projection", proj);
//   for (auto& mesh : meshes_) {
//     mesh->Bind();
//     sprite->SetMat4("model", model);
//     glDrawElements(static_cast<GLenum>(mesh->GetTopology()), mesh->Count(),
//                    GL_UNSIGNED_INT, 0);
//   }

//   gui_->Render();
// }

// void Scene::RenderScene(glm::vec4 clip_plane) {
//   ShaderHandle terrainShader = ResourceManager::GetShader("terrain");
//   ShaderHandle skyboxShader = ResourceManager::GetShader("skybox");

//   terrainShader->Use();
//   terrainShader->SetMat4("view", camera.getViewMatrix());
//   terrainShader->SetMat4("projection", camera.getProjectionMatrix());
//   terrainShader->SetVec3("light.direction",
//                          glm::normalize(light.GetDirection()));
//   terrainShader->SetVec3("light.color", light.GetColor());
//   terrainShader->SetFloat("light.intensity", light.GetIntensity());
//   terrainShader->SetVec4("clipPlane", clip_plane);
//   terrain_->Draw(terrainShader);
// }

void Scene::OnKeyEvent(int key, int scancode, int action, int mode) {
  gui_->OnKeyEvent(key, scancode, action, mode);

  if (action == GLFW_PRESS) {
    SetKeyPressed(key);
  } else if (action == GLFW_RELEASE) {
    SetKeyReleased(key);
  }
  if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
    camera.Toggle();
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

  camera.handleMouseMovement(offsetX, offsetY);

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
