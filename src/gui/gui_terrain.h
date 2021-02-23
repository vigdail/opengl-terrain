#pragma once

#include "gui_panel.h"
#include "../terrain.h"
#include <memory>

class GUITerrainPanel : public GUIPanel {
 public:
  explicit GUITerrainPanel(std::shared_ptr<Terrain> terrain);
  void Render() override;

 private:
  std::shared_ptr<Terrain> terrain_;
};
