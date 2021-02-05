#pragma once

#include "texture.h"

class NormalmapRenderer {
 public:
  NormalmapRenderer();
  Texture Render(const Texture &heightmap);

 private:
};
