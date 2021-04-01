#pragma once

#include <filesystem>
#include <map>
#include <optional>
#include <string>

#include "shader.h"
#include "texture.h"

namespace fs = std::filesystem;

using ShaderHandle = Shader *;
using TextureHandle = Texture *;

class ResourceManager {
 private:
  static std::map<std::string, Shader> shaders_;
  static std::map<std::string, Texture> textures_;

 public:
  ResourceManager() = delete;
  static void AddShader(std::string name, const ShaderBuilder &builder);
  static ShaderHandle GetShader(std::string name);

  static void AddTexture(std::string name, const TextureBuilder &bulder);
  static TextureHandle GetTexture(std::string name);
};
