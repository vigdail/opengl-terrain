#pragma once

#include <filesystem>
#include <map>
#include <optional>
#include <string>

#include "shader.h"
#include "texture.h"

namespace fs = std::filesystem;

using ShaderHandle = Shader *;

// @TODO: Refactor shader loading
class ResourceManager {
 private:
  static std::map<std::string, Shader> shaders_;
  static std::map<std::string, Texture> textures_;

  static Texture loadTextureFromFile(const std::filesystem::path &path);

 public:
  ResourceManager() = delete;
  static void AddShader(std::string name, const ShaderBuilder &builder);
  static ShaderHandle GetShader(std::string name);

  static void LoadTexture(std::string name, const std::filesystem::path &path);
  static Texture &GetTexture(std::string name);
};
