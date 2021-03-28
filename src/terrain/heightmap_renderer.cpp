#include "heightmap_renderer.h"

#include <iostream>
#include "../resource_manager.h"

HeightmapRenderer::HeightmapRenderer() {}

Texture HeightmapRenderer::Render(int width, int height, int octaves) {
  TextureViewDescriptor view{};
  view.internal_format = GL_R32F;
  view.image_format = GL_RED;
  view.type = GL_FLOAT;
  view.width = width;
  view.height = height;
  Texture heightmap = TextureBuilder().WithView(view).Build();
  // heightmap.internal_format = GL_R32F;
  // heightmap.image_format = GL_RED;
  // heightmap.type = GL_FLOAT;
  // heightmap.Generate(width, height, NULL);
  heightmap.BindImage();

  ShaderHandle shader = ResourceManager::GetShader("compute_heightmap");
  shader->Use();
  glActiveTexture(GL_TEXTURE0);
  shader->SetInt("width", width);
  shader->SetInt("octaves", octaves);
  glDispatchCompute(width / 16, height / 16, 1);

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  return heightmap;
}
