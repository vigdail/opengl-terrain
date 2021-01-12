#include "game.h"

#include <glm/glm.hpp>
#include <iostream>
#include <string>

#include "application.h"

const uint Game::kKeysCount_;

Game::Game(uint width, uint height)
    : width_(width),
      height_(height),
      state_(GAME_MENU),
      keys_(),
      terrain_(Terrain()) {
  LoadAssets();
}

Game::~Game() { ResourceManager::Clear(); }

void Game::LoadAssets() {
  ResourceManager::LoadShader("terrain", "../assets/shaders/terrain.vs",
                              "../assets/shaders/terrain.fs");
}

void Game::ProcessInput(float dt) {}

void Game::Update(float dt) {}

void Game::Render() {
  Shader terrainShader = ResourceManager::GetShader("terrain");
  terrain_.Draw(terrainShader);
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
