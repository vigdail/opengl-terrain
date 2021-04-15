
#include "gui_sun.h"

GuiSunPanel::GuiSunPanel(DirectionalLight *sun)
    : GuiPanel(), hour_(8), sun_(sun) {
  auto angle = timeToAngle();
  sun_->setPosition(glm::vec3(cos(angle), sin(angle), 0.0f));
}

void GuiSunPanel::render() {
  if (!ImGui::CollapsingHeader("Sun")) {
    return;
  }
  if (ImGui::SliderFloat("Hour", &hour_, 5, 19, "%.1f")) {
    auto angle = timeToAngle();
    sun_->setPosition(glm::vec3(cos(angle), sin(angle), 0.0f));
  }
}

float GuiSunPanel::timeToAngle() const {
  return (hour_ - 6.0f) / 12.0f * M_PI;
}
