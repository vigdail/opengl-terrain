#include "heightmap_renderer.h"

#include <iostream>
#include "resource_manager.h"

HeightmapRenderer::HeightmapRenderer() {}

Texture HeightmapRenderer::Render(int width, int height, int octaves) {
  Texture heightmap;
  heightmap.internal_format = GL_RGBA32F;
  heightmap.image_format = GL_RGBA;
  heightmap.type = GL_FLOAT;
  heightmap.Generate(width, height, NULL);
  heightmap.BindImage();

  Shader& shader = ResourceManager::GetShader("compute_heightmap");
  shader.Use();
  glActiveTexture(GL_TEXTURE0);
  shader.SetInt("width", width);
  shader.SetInt("octaves", octaves);
  glDispatchCompute(width / 16, height / 16, 1);

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  return heightmap;
}
