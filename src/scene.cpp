#include "scene.h"
#include "application.h"

#include "gui/gui_skybox.h"
#include "gui/gui_sun.h"
#include "gui/gui_terrain.h"
#include "gui/gui_water.h"
#include <glm/glm.hpp>
#include <string>

const uint Scene::keys_count_;

Scene::Scene(uint width, uint height)
    : camera(Camera(60.0f, 1.0f * width / height, 0.1f, 3000.0f)),
      light(DirectionalLight(glm::vec3(0.0f), glm::vec3(0.0f))),
      width_(width), height_(height), keys_(), mouse_last_x_(0.0), mouse_last_y_(0.0) {
  loadAssets();
  camera.position = glm::vec3(0.0f, 3.0f, -2.0f);
  terrain = std::make_shared<Terrain>(TerrainConfig{});
  skybox = std::make_unique<Skybox>();
  water = std::make_shared<Water>();
  terrain->update(camera);

  gui = std::make_unique<GuiLayer>(width, height);
  gui->addPanel(new GuiSkyboxPanel(skybox->getAtmosphere()));
  gui->addPanel(new GuiSunPanel(&light));
  gui->addPanel(new GuiWaterPanel(water));
  gui->addPanel(new GuiTerrainPanel(terrain));
}

void Scene::loadAssets() {
  ResourceManager::addShader(
      "terrain",
      ShaderBuilder()
          .load("../assets/shaders/terrain.vert", ShaderModule::Type::VERTEX)
          .load("../assets/shaders/terrain.frag", ShaderModule::Type::FRAGMENT)
          .load("../assets/shaders/terrain.tesc", ShaderModule::Type::TESS_CONTROL)
          .load("../assets/shaders/terrain.tese", ShaderModule::Type::TESS_EVALUATION));
  ResourceManager::addShader(
      "skybox",
      ShaderBuilder()
          .load("../assets/shaders/skybox.vert", ShaderModule::Type::VERTEX)
          .load("../assets/shaders/skybox.frag", ShaderModule::Type::FRAGMENT));
  ResourceManager::addShader(
      "solid",
      ShaderBuilder()
          .load("../assets/shaders/solid_color.vert", ShaderModule::Type::VERTEX)
          .load("../assets/shaders/solid_color.frag",
                ShaderModule::Type::FRAGMENT));
  ResourceManager::addShader(
      "sprite",
      ShaderBuilder()
          .load("../assets/shaders/sprite.vert", ShaderModule::Type::VERTEX)
          .load("../assets/shaders/sprite.frag", ShaderModule::Type::FRAGMENT));
  ResourceManager::addShader(
      "water",
      ShaderBuilder()
          .load("../assets/shaders/water.vert", ShaderModule::Type::VERTEX)
          .load("../assets/shaders/water.frag", ShaderModule::Type::FRAGMENT));

  ResourceManager::addShader(
      "compute_normalmap",
      ShaderBuilder().load("../assets/shaders/compute/normalmap.comp",
                           ShaderModule::Type::COMPUTE));
  ResourceManager::addShader(
      "compute_heightmap",
      ShaderBuilder().load("../assets/shaders/compute/heightmap.comp",
                           ShaderModule::Type::COMPUTE));

  TextureSamplerDescriptor sampler{};
  sampler.wrap_s = GL_REPEAT;
  sampler.wrap_t = GL_REPEAT;
  ResourceManager::addTexture("water_dudv",
                              TextureBuilder()
                                  .load("../assets/textures/water_dudv.png")
                                  .withSampler(sampler));
  ResourceManager::addTexture(
      "water_normal", TextureBuilder().load("../assets/textures/water_normalmap.png").withSampler(sampler));
}

void Scene::processInput(float dt) {
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

void Scene::update(float dt) {
  //  camera.position.y =
  //      terrain->getHeight(camera.position.x, camera.position.z) + 1.7f;
  terrain->update(camera);
  gui->update(dt);
}

void Scene::onKeyEvent(int key, int scancode, int action, int mode) {
  gui->onKeyEvent(key, scancode, action, mode);

  if (action == GLFW_PRESS) {
    setKeyPressed(key);
  } else if (action == GLFW_RELEASE) {
    setKeyReleased(key);
  }
  if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
    camera.toggle();
  }
}

void Scene::onMouseButtonEvent(int button, int action, int mode) const {
  gui->onMouseButtonEvent(button, action, mode);
}

void Scene::onMousePositionEvent(double x, double y) {
  auto offset_x = static_cast<float>(x - mouse_last_x_);
  auto offset_y = static_cast<float>(mouse_last_y_ - y);

  mouse_last_x_ = x;
  mouse_last_y_ = y;

  camera.handleMouseMovement(offset_x, offset_y);

  gui->onMousePositionEvent(x, y);
}

void Scene::setKeyPressed(uint key) {
  if (key < keys_count_) {
    keys_[key] = true;
  }
}

void Scene::setKeyReleased(uint key) {
  if (key < keys_count_) {
    keys_[key] = false;
  }
}

bool Scene::isKeyPressed(uint key) {
  if (key < keys_count_) {
    return keys_[key];
  }

  return false;
}
uint32_t Scene::getWidth() const {
  return width_;
}
uint32_t Scene::getHeight() const {
  return height_;
}
