
#include "gui_skybox.h"

GUISkyboxPanel::GUISkyboxPanel(Atmosphere &atmosphere)
    : atmosphere_(atmosphere) {}

void GUISkyboxPanel::Render() {
  ImGui::Begin("Atmosphere");
  ImGui::SliderFloat("Br", &atmosphere_.Br, 0.0f, 0.01f);
  ImGui::SliderFloat("Bm", &atmosphere_.Bm, 0.0f, 0.008f);
  ImGui::SliderFloat("g", &atmosphere_.g, -0.999f, 0.999f);
  ImGui::SliderFloat3("color", &atmosphere_.color.r, 0.0f, 1.0f);
  ImGui::End();
}
