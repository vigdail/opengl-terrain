
#include "gui_skybox.h"

GuiSkyboxPanel::GuiSkyboxPanel(Atmosphere &atmosphere)
    : GuiPanel(), atmosphere_(atmosphere) {}

void GuiSkyboxPanel::render() {
  if (!ImGui::CollapsingHeader("Atmosphere")) {
    return;
  }
  ImGui::PushID("atmosphere");
  ImGui::SliderFloat("Planet radius", &atmosphere_.planet_radius, 1000e3,
                     7000e3, "%.0f");
  ImGui::SliderFloat("Atmosphere radius", &atmosphere_.atmosphere_radius,
                     1000e3, 7000e3, "%.0f");
  ImGui::SliderFloat("Rayleigh height", &atmosphere_.h_r, 100, 10000, "%.0f");
  ImGui::SliderFloat("Mie height", &atmosphere_.h_m, 100, 10000, "%.0f");
  ImGui::SliderFloat("g", &atmosphere_.g, -0.999f, 0.999f);
  glm::vec3 rayleigh_coeffs(atmosphere_.beta_r * 10.0e6f);
  if (ImGui::SliderFloat3("Rayleigh coeffs", &rayleigh_coeffs.r, 0.0f, 500.0f, "%.0f")) {
    atmosphere_.beta_r = rayleigh_coeffs / 10.0e6f;
  }
  float mie_coeff = atmosphere_.beta_m * 10.0e6f;
  if (ImGui::SliderFloat("Mie coeffs", &mie_coeff, 0.0f, 500.0f, "%.0f")) {
    atmosphere_.beta_m = mie_coeff / 10.0e6f;
  }
  ImGui::SliderInt("View dir samples", &atmosphere_.view_dir_samples, 1, 100);
  ImGui::SliderInt("Sun dir samples", &atmosphere_.sun_dir_samples, 1, 100);
  ImGui::PopID();
}
