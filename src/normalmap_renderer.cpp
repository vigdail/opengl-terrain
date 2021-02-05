#include "normalmap_renderer.h"

#include <iostream>
#include "resource_manager.h"

NormalmapRenderer::NormalmapRenderer() {}

Texture NormalmapRenderer::Render(const Texture& heightmap) {
  Texture normalmap;
  normalmap.internal_format = GL_RGBA32F;
  normalmap.image_format = GL_RGBA;
  normalmap.type = GL_FLOAT;
  normalmap.Generate(heightmap.GetWidth(), heightmap.GetHeight(), NULL);
  normalmap.BindImage();

  Shader& shader = ResourceManager::GetShader("compute_normalmap");
  shader.Use();
  glActiveTexture(GL_TEXTURE0);
  heightmap.Bind();
  shader.SetInt("heightmap", 0);
  shader.SetInt("width", heightmap.GetWidth());
  shader.SetFloat("strength", 10.0f);
  glDispatchCompute(heightmap.GetWidth() / 16, heightmap.GetHeight() / 16, 1);

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  return normalmap;
}
