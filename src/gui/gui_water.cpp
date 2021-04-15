#include "gui_water.h"

#include <imgui/imgui.h>

#include <utility>

GuiWaterPanel::GuiWaterPanel(std::shared_ptr<Water> water)
    : water_(std::move(water)) {}

void GuiWaterPanel::render() {
  if (!ImGui::CollapsingHeader("Water")) {
    return;
  }
  ImGui::PushID("water");
  float height = water_->getHeight();
  if (ImGui::SliderFloat("Height", &height, 0.0f, 150.0f, "%.1f")) {
    water_->setHeight(height);
  }
  WaterMaterial &material = water_->getMaterial();
  ImGui::SliderFloat("Specular", &material.specular_power, 4.0, 32.0, "%.0f");
  ImGui::SliderFloat("Reflective", &material.reflection_power, 0.1, 1.0, "%.05f");
  ImGui::SliderFloat("Wave Strength", &material.wave_strength, 0.0, 0.05, "%.05f");
  ImGui::SliderFloat("Wave Speed Factor", &material.wave_speed_factor, 1, 1000, "%.0f");
  ImGui::SliderFloat("DUDV Tiling", &material.dudv_tiling, 1.0, 100.0, "%.0f");
  ImGui::SliderFloat("Depth factor", &material.depth_factor, 0.0, 10.0, "%.1f");
  ImGui::ColorEdit3("Color", &material.color.x);
  ImGui::PopID();
}
