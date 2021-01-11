#pragma once

#include <vector>

#include <entt/entt.hpp>

#include "resource_manager.h"

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

 private:
  static const uint kKeysCount_ = 1024;
  entt::registry registry_;
  uint width_;
  uint height_;
  GameState state_;
  bool keys_[kKeysCount_];
};
