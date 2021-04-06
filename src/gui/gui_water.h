#pragma once

#include "../water/water.h"
#include "gui_panel.h"

#include <memory>

class GuiWaterPanel : public GuiPanel {
 public:
  explicit GuiWaterPanel(std::shared_ptr<Water> water);
  void render() override;

 private:
  std::shared_ptr<Water> water_;
};
