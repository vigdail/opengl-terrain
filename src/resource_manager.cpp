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

void ResourceManager::AddTexture(std::string name,
                                 const TextureBuilder &builder) {
  textures_.emplace(name, builder.Build());
}
TextureHandle ResourceManager::GetTexture(std::string name) {
  return &textures_[name];
}
