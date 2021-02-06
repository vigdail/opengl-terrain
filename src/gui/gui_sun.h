#pragma once

#include "gui_panel.h"
#include "../light/directional_light.h"
#include <imgui/imgui.h>

class GUISunPanel : public GUIPanel {
 public:
  explicit GUISunPanel(DirectionalLight *sun);

  void Render() override;

 private:
  float hour_;
  DirectionalLight *sun_;
};
