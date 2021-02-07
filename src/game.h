#pragma once

#include <vector>
#include <memory>

#include <entt/entt.hpp>

#include "camera.h"
#include "light/directional_light.h"
#include "resource_manager.h"
#include "terrain.h"
#include "skybox.h"
#include "water/water.h"
#include "gui/gui_layer.h"

class Game {
 public:
  Game(uint width, uint height);
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

 private:
  static const uint kKeysCount_ = 1024;
  entt::registry registry_;
  uint width_;
  uint height_;
  bool keys_[kKeysCount_];
  Camera camera_;
  DirectionalLight light_;
  std::unique_ptr<Terrain> terrain_;
  std::unique_ptr<GUILayer> gui_;
  std::unique_ptr<Skybox> skybox_;
  std::unique_ptr<Water> water_;
  double mouse_last_x_;
  double mouse_last_y_;
};
