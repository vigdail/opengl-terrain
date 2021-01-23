#pragma once

#include "texture.h"

class NormalmapRenderer {
 public:
  NormalmapRenderer();
  Texture Render(Texture &heightmap);

 private:
};
