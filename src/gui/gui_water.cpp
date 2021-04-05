#include "gui_water.h"

#include <imgui/imgui.h>

#include <utility>

GuiWaterPanel::GuiWaterPanel(std::shared_ptr<WaterRenderer> water)
    : water_(std::move(water)) {}

void GuiWaterPanel::render() {
  ImGui::Begin("Water");
  float height = water_->getHeight();
  if (ImGui::SliderFloat("Height", &height, 0.0f, 150.0f, "%.1f")) {
    water_->setHeight(height);
  }
  WaterMaterial &material = water_->getMaterial();
  ImGui::SliderFloat("Specular", &material.specular_power, 4.0, 128.0, "%.0f");
  ImGui::SliderFloat("Reflective", &material.reflection_power, 0.1, 10.0, "%.1f");
  ImGui::SliderFloat("DUDV Tiling", &material.dudv_tiling, 1.0, 20.0, "%.0f");
  ImGui::End();
}
