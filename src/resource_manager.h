#pragma once

#include <filesystem>
#include <map>
#include <optional>
#include <string>

#include "shader.h"
#include "texture.h"

namespace fs = std::filesystem;

// @TODO: Refactor shader loading
class ResourceManager {
 private:
  static std::map<std::string, Shader> shaders_;
  static std::map<std::string, Texture> textures_;

  static Shader loadComputeShaderFromFile(const std::filesystem::path &path);
  static Shader loadShaderFromFile(
      const std::filesystem::path &v_shader_path,
      const std::filesystem::path &f_shader_path,
      const std::optional<std::filesystem::path> g_shader_path);
  static Texture loadTextureFromFile(const std::filesystem::path &path);

 public:
  ResourceManager() = delete;
  static void LoadComputeShader(std::string name,
                                const std::filesystem::path &path);
  static void LoadShader(
      std::string name, const std::filesystem::path &v_shader_path,
      const std::filesystem::path &f_shader_path,
      const std::optional<std::filesystem::path> g_shader_path = std::nullopt);
  static Shader &GetShader(std::string name);

  static void LoadTexture(std::string name, const std::filesystem::path &path);
  static Texture &GetTexture(std::string name);
};
