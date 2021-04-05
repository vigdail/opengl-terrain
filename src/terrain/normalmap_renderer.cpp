#include "normalmap_renderer.h"

#include "../resource_manager.h"

Texture NormalmapRenderer::render(const Texture &heightmap) {
  TextureViewDescriptor view{};
  view.internal_format = GL_RGBA32F;
  view.image_format = GL_RGBA;
  view.type = GL_FLOAT;
  view.width = heightmap.getWidth();
  view.height = heightmap.getHeight();
  Texture normalmap = TextureBuilder().withView(view).build();
  normalmap.bindImage();

  ShaderHandle shader = ResourceManager::getShader("compute_normalmap");
  shader->use();
  glActiveTexture(GL_TEXTURE0);
  heightmap.bind();
  shader->setInt("heightmap", 0);
  shader->setInt("width", heightmap.getWidth());
  shader->setFloat("strength", 10.0f);
  glDispatchCompute(heightmap.getWidth() / 16, heightmap.getHeight() / 16, 1);

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  return normalmap;
}
