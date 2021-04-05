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
  static void addShader(const std::string &name, const ShaderBuilder &builder);
  static ShaderHandle getShader(const std::string &name);

  static void addTexture(const std::string &name, const TextureBuilder &builder);
  static TextureHandle getTexture(const std::string &name);
};
