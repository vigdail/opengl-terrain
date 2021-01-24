#pragma once

#include "texture.h"

class HeightmapRenderer {
 public:
  HeightmapRenderer();
  Texture Render(int width, int height, int octaves);

 private:
};
