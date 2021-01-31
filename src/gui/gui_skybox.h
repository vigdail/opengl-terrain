#pragma once

#include "../skybox.h"
#include <imgui/imgui.h>

class GUISkyboxPanel {
 public:
  explicit GUISkyboxPanel(Atmosphere &atmosphere);

  void Render();

 private:
  Atmosphere &atmosphere_;
};
