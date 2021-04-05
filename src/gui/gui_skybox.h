#pragma once

#include "../skybox/skybox.h"
#include "gui_panel.h"
#include <imgui/imgui.h>

class GuiSkyboxPanel : public GuiPanel {
 public:
  explicit GuiSkyboxPanel(Atmosphere &atmosphere);

  void render() override;

 private:
  Atmosphere &atmosphere_;
};
