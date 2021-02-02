#include "resource_manager.h"

#include <stb_image.h>

#include <fstream>
#include <iostream>
#include <sstream>

std::map<std::string, Texture> ResourceManager::textures_;
std::map<std::string, Shader> ResourceManager::shaders_;

void ResourceManager::LoadComputeShader(std::string name,
                                        const std::filesystem::path &path) {
  shaders_.emplace(name, loadComputeShaderFromFile(path));
}
void ResourceManager::LoadShader(
    std::string name, const std::filesystem::path &v_shader_path,
    const std::filesystem::path &f_shader_path,
    const std::optional<std::filesystem::path> g_shader_path) {
  shaders_.emplace(
      name, loadShaderFromFile(v_shader_path, f_shader_path, g_shader_path));
}

Shader &ResourceManager::GetShader(std::string name) { return shaders_[name]; }

void ResourceManager::LoadTexture(std::string name,
                                  const std::filesystem::path &path) {
  textures_.emplace(name, loadTextureFromFile(path));
}
Texture &ResourceManager::GetTexture(std::string name) {
  return textures_[name];
}

Shader ResourceManager::loadShaderFromFile(
    const std::filesystem::path &v_shader_path,
    const std::filesystem::path &f_shader_path,
    const std::optional<std::filesystem::path> g_shader_path) {
  std::string vertex_code;
  std::string fragment_code;
  std::string geom_code;

  try {
    std::ifstream vertex_file(v_shader_path);
    std::ifstream fragment_file(f_shader_path);
    std::stringstream vertex_stream;
    std::stringstream fragment_stream;

    vertex_file.exceptions(std::fstream::failbit | std::fstream::badbit);
    fragment_file.exceptions(std::fstream::failbit | std::fstream::badbit);

    vertex_stream << vertex_file.rdbuf();
    fragment_stream << fragment_file.rdbuf();

    vertex_file.close();
    fragment_file.close();

    vertex_code = vertex_stream.str();
    fragment_code = fragment_stream.str();

    if (g_shader_path.has_value()) {
      std::ifstream geom_file(*g_shader_path);
      std::stringstream geom_stream;

      geom_file.exceptions(std::fstream::failbit | std::fstream::badbit);

      geom_stream << geom_file.rdbuf();
      geom_file.close();

      geom_code = geom_stream.str();
    }
  } catch (std::ifstream::failure &e) {
    std::cerr << "ERROR::SHADER: Can't load shader from file: "
              << e.code().message() << std::endl;
  }

  const char *v_shader_code = vertex_code.c_str();
  const char *f_shader_code = fragment_code.c_str();
  const char *g_shader_code =
      (g_shader_path.has_value() ? geom_code.c_str() : nullptr);

  Shader shader;
  shader.AttachShader(GL_VERTEX_SHADER, v_shader_code);
  shader.AttachShader(GL_FRAGMENT_SHADER, f_shader_code);
  if (g_shader_code) {
    shader.AttachShader(GL_GEOMETRY_SHADER, g_shader_code);
  }
  shader.Link();

  return shader;
}

Shader ResourceManager::loadComputeShaderFromFile(
    const std::filesystem::path &path) {
  std::string code;

  try {
    std::ifstream file(path);
    std::stringstream stream;

    file.exceptions(std::fstream::failbit | std::fstream::badbit);

    stream << file.rdbuf();

    file.close();

    code = stream.str();
  } catch (std::ifstream::failure &e) {
    std::cerr << "ERROR::SHADER: Can't load shader from file: "
              << e.code().message() << std::endl;
  }

  const char *shader_code = code.c_str();

  Shader shader;
  shader.AttachShader(GL_COMPUTE_SHADER, shader_code);
  shader.Link();

  return shader;
}

Texture ResourceManager::loadTextureFromFile(
    const std::filesystem::path &path) {
  int width;
  int height;
  int n_channels;

  unsigned char *data =
      stbi_load(path.c_str(), &width, &height, &n_channels, 0);

  if (!data) {
    std::cerr << "ERROR::TEXTURE: Unable to load texture from file: " << path
              << std::endl;
  }

  Texture texture;
  if (n_channels == 1) {
    texture.image_format = GL_RED;
    texture.internal_format = GL_RED;
  } else if (n_channels == 4) {
    texture.image_format = GL_RGBA;
    texture.internal_format = GL_RGBA;
  }

  texture.Generate(width, height, data);
  stbi_image_free(data);

  return texture;
}
