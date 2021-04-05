#pragma once

#include "../light/directional_light.h"
#include "gui_panel.h"

#include <imgui/imgui.h>

class GuiSunPanel : public GuiPanel {
 public:
  explicit GuiSunPanel(DirectionalLight *sun);
  void render() override;

 private:
  float hour_;
  DirectionalLight *sun_;
  float timeToAngle() const;
};
