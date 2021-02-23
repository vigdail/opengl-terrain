#include "gui_water.h"
#include <imgui/imgui.h>

GUIWaterPanel::GUIWaterPanel(std::shared_ptr<WaterRenderer> water)
    : water_(water) {}

void GUIWaterPanel::Render() {
  ImGui::Begin("Water");
  float height = water_->GetHeight();
  if (ImGui::SliderFloat("Height", &height, 0.0f, 150.0f, "%.1f")) {
    water_->SetHeight(height);
  }
  WaterMaterial& material = water_->GetMaterial();
  ImGui::SliderFloat("Specular", &material.specular_power, 4.0, 128.0, "%.0f");
  ImGui::SliderFloat("Reflective", &material.reflection_power, 0.1, 10.0,
                     "%.1f");
  ImGui::SliderFloat("DUDV Tiling", &material.dudv_tiling, 1.0, 20.0, "%.0f");
  ImGui::End();
}
