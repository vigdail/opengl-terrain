#include "gui_terrain.h"
#include <imgui/imgui.h>

#include <utility>

GuiTerrainPanel::GuiTerrainPanel(std::shared_ptr<Terrain> terrain)
    : terrain_(std::move(terrain)) {}

void GuiTerrainPanel::render() {
  if (!ImGui::CollapsingHeader("Terrain")) {
    return;
  }
  float scale_y = terrain_->getScaleY();
  if (ImGui::SliderFloat("Height", &scale_y, 0.0f, 500.0f, "%.0f")) {
    terrain_->setScaleY(scale_y);
  }
}
