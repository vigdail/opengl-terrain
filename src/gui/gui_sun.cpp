
#include "gui_sun.h"

GUISunPanel::GUISunPanel(DirectionalLight *sun)
    : GUIPanel(), hour_(8), sun_(sun) {
  float angle = (hour_ - 6) / 12 * M_PI;
  sun_->SetPosition(glm::vec3(cos(angle), sin(angle), 0.0f));
}

void GUISunPanel::Render() {
  ImGui::Begin("Sun");
  if (ImGui::SliderFloat("Hour", &hour_, 5, 19, "%.1f")) {
    float angle = (hour_ - 6) / 12 * M_PI;
    sun_->SetPosition(glm::vec3(cos(angle), sin(angle), 0.0f));
  }

  ImGui::End();
}
