#include "normalmap_renderer.h"

#include <iostream>
#include "../resource_manager.h"

NormalmapRenderer::NormalmapRenderer() {}

Texture NormalmapRenderer::Render(const Texture& heightmap) {
  TextureViewDescriptor view{};
  view.internal_format = GL_RGBA32F;
  view.image_format = GL_RGBA;
  view.type = GL_FLOAT;
  view.width = heightmap.GetWidth();
  view.height = heightmap.GetHeight();
  Texture normalmap = TextureBuilder().WithView(view).Build();
  normalmap.BindImage();

  ShaderHandle shader = ResourceManager::GetShader("compute_normalmap");
  shader->Use();
  glActiveTexture(GL_TEXTURE0);
  heightmap.Bind();
  shader->SetInt("heightmap", 0);
  shader->SetInt("width", heightmap.GetWidth());
  shader->SetFloat("strength", 10.0f);
  glDispatchCompute(heightmap.GetWidth() / 16, heightmap.GetHeight() / 16, 1);

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  return normalmap;
}
