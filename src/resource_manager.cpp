#include "resource_manager.h"

#include <stb_image.h>

#include <fstream>
#include <iostream>
#include <sstream>

std::map<std::string, Texture> ResourceManager::textures_;
std::map<std::string, Shader> ResourceManager::shaders_;

void ResourceManager::AddShader(std::string name,
                                const ShaderBuilder &builder) {
  shaders_.emplace(name, builder.Build());
}

ShaderHandle ResourceManager::GetShader(std::string name) {
  return &shaders_[name];
}

void ResourceManager::LoadTexture(std::string name,
                                  const std::filesystem::path &path) {
  textures_.emplace(name, loadTextureFromFile(path));
}
Texture &ResourceManager::GetTexture(std::string name) {
  return textures_[name];
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
