#include "heightmap_renderer.h"

#include "../resource_manager.h"

Texture HeightmapRenderer::render(int width, int height, int octaves) {
  TextureViewDescriptor view{};
  view.internal_format = GL_R32F;
  view.image_format = GL_RED;
  view.type = GL_FLOAT;
  view.width = width;
  view.height = height;
  Texture heightmap = TextureBuilder().withView(view).build();
  heightmap.bindImage();

  ShaderHandle shader = ResourceManager::getShader("compute_heightmap");
  shader->use();
  glActiveTexture(GL_TEXTURE0);
  shader->setInt("width", width);
  shader->setInt("octaves", octaves);
  glDispatchCompute(width / 16, height / 16, 1);

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  return heightmap;
}
