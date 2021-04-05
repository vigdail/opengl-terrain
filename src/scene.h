#pragma once

#include <memory>
#include <vector>

#include <entt/entt.hpp>

#include "camera.h"
#include "gui/gui_layer.h"
#include "light/directional_light.h"
#include "mesh/shape/quad.h"
#include "resource_manager.h"
#include "skybox/skybox.h"
#include "terrain/terrain.h"
#include "water/water_renderer.h"

class Scene {
 public:
  Scene(uint32_t width, uint32_t height);
  void processInput(float dt);
  void update(float dt);
  void setKeyPressed(uint32_t key);
  void setKeyReleased(uint32_t key);
  bool isKeyPressed(uint32_t key);
  void onKeyEvent(int key, int scancode, int action, int mode);
  void onMouseButtonEvent(int button, int action, int mode);
  void onMousePositionEvent(double x, double y);

  Camera camera;
  DirectionalLight light;
  std::unique_ptr<Skybox> skybox;

 private:
  static const uint32_t keys_count_ = 1024;
  entt::registry registry_;
  uint32_t width_;
  uint32_t height_;
  bool keys_[keys_count_];
  std::shared_ptr<Terrain> terrain_;
  std::unique_ptr<GuiLayer> gui_;
  std::shared_ptr<WaterRenderer> water_;
  std::vector<std::shared_ptr<Mesh>> meshes_;

  double mouse_last_x_;
  double mouse_last_y_;

 private:
  static void loadAssets();
  //  void RenderScene(glm::vec4 clip_plane);
};
