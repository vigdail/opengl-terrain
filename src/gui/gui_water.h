#pragma once

#include "gui_panel.h"
#include "../water/water_renderer.h"
#include <imgui/imgui.h>
#include <memory>

class GUIWaterPanel : public GUIPanel {
 public:
  explicit GUIWaterPanel(std::shared_ptr<WaterRenderer> water);
  void Render() override;

 private:
  std::shared_ptr<WaterRenderer> water_;
};
