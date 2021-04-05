#pragma once

#include "../water/water_renderer.h"
#include "gui_panel.h"

#include <memory>

class GuiWaterPanel : public GuiPanel {
 public:
  explicit GuiWaterPanel(std::shared_ptr<WaterRenderer> water);
  void render() override;

 private:
  std::shared_ptr<WaterRenderer> water_;
};
