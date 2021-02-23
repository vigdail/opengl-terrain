#include "gui_terrain.h"
#include <imgui/imgui.h>

GUITerrainPanel::GUITerrainPanel(std::shared_ptr<Terrain> terrain)
    : terrain_(terrain) {}

void GUITerrainPanel::Render() {
  ImGui::Begin("Terrain");
  float scale_y = terrain_->GetScaleY();
  if (ImGui::SliderFloat("Height", &scale_y, 0.0f, 500.0f, "%.0f")) {
    terrain_->SetScaleY(scale_y);
  }
  ImGui::End();
}
