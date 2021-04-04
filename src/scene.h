#pragma once

#include <vector>
#include <memory>

#include <entt/entt.hpp>

#include "camera.h"
#include "light/directional_light.h"
#include "resource_manager.h"
#include "terrain/terrain.h"
#include "skybox/skybox.h"
#include "water/water_renderer.h"
#include "gui/gui_layer.h"
#include "mesh/shape/quad.h"

class Scene {
 public:
  Scene(uint width, uint height);
  void LoadAssets();
  void ProcessInput(float dt);
  void Update(float dt);
  void Render();
  void SetKeyPressed(uint key);
  void SetKeyReleased(uint key);
  bool IsKeyPressed(uint key);
  void OnKeyEvent(int key, int scancode, int action, int mode);
  void OnMouseButtonEvent(int button, int action, int mode);
  void OnMousePositionEvent(double x, double y);

  Camera camera;
  DirectionalLight light;
  std::unique_ptr<Skybox> skybox;

 private:
  static const uint kKeysCount_ = 1024;
  entt::registry registry_;
  uint width_;
  uint height_;
  bool keys_[kKeysCount_];
  std::shared_ptr<Terrain> terrain_;
  std::unique_ptr<GUILayer> gui_;
  std::shared_ptr<WaterRenderer> water_;
  std::vector<std::shared_ptr<Mesh>> meshes_;

  double mouse_last_x_;
  double mouse_last_y_;

 private:
  void RenderScene(glm::vec4 clip_plane);
};
