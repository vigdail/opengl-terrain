#pragma once

#include "../skybox/skybox.h"
#include "gui_panel.h"
#include <imgui/imgui.h>

class GUISkyboxPanel : public GUIPanel {
 public:
  explicit GUISkyboxPanel(Atmosphere &atmosphere);

  void Render() override;

 private:
  Atmosphere &atmosphere_;
};
