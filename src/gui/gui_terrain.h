#pragma once

#include "../terrain/terrain.h"
#include "gui_panel.h"

#include <memory>

class GuiTerrainPanel : public GuiPanel {
 public:
  explicit GuiTerrainPanel(std::shared_ptr<Terrain> terrain);
  void render() override;

 private:
  std::shared_ptr<Terrain> terrain_;
};
