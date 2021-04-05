#include "resource_manager.h"

#include <iostream>

std::map<std::string, Texture> ResourceManager::textures_;
std::map<std::string, Shader> ResourceManager::shaders_;

void ResourceManager::addShader(const std::string &name,
                                const ShaderBuilder &builder) {
  shaders_.emplace(name, builder.build());
}

ShaderHandle ResourceManager::getShader(const std::string &name) {
  return &shaders_[name];
}

void ResourceManager::addTexture(const std::string &name,
                                 const TextureBuilder &builder) {
  textures_.emplace(name, builder.build());
}
TextureHandle ResourceManager::getTexture(const std::string &name) {
  return &textures_[name];
}
