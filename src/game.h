#pragma once

#include <vector>

#include <entt/entt.hpp>

#include "camera.h"
#include "light/directional_light.h"
#include "resource_manager.h"
#include "terrain.h"

enum GameState {
  GAME_MENU,
  GAME_ACTIVE,
  GAME_WIN,
};

class Game {
 public:
  Game(uint width, uint height);
  ~Game();
  void LoadAssets();
  void ProcessInput(float dt);
  void Update(float dt);
  void Render();
  void SetKeyPressed(uint key);
  void SetKeyReleased(uint key);
  bool IsKeyPressed(uint key);
  void MouseCallback(double x, double y);

 private:
  static const uint kKeysCount_ = 1024;
  entt::registry registry_;
  uint width_;
  uint height_;
  GameState state_;
  bool keys_[kKeysCount_];
  Camera camera_;
  DirectionalLight light_;
  Terrain terrain_;
  double mouse_last_x_;
  double mouse_last_y_;
};
