
#include "gui_skybox.h"

GUISkyboxPanel::GUISkyboxPanel(Atmosphere &atmosphere)
    : GUIPanel(), atmosphere_(atmosphere) {}

void GUISkyboxPanel::Render() {
  ImGui::Begin("Atmosphere");
  ImGui::SliderInt("Planet radius", &atmosphere_.planet_radius, 1000e3, 7000e3);
  ImGui::SliderInt("Atmosphere radius", &atmosphere_.atmosphere_radius, 1000e3,
                   7000e3);
  ImGui::SliderFloat("Rayleigh height", &atmosphere_.hR, 100, 10000, "%.0f");
  ImGui::SliderFloat("Mie height", &atmosphere_.hM, 100, 10000, "%.0f");
  ImGui::SliderFloat("g", &atmosphere_.g, -0.999f, 0.999f);
  glm::vec3 rayleigh_coeffs(atmosphere_.beta_R * 10.0e6f);
  if (ImGui::SliderFloat3("Rayleigh coeffs", &rayleigh_coeffs.r, 0.0f, 500.0f,
                          "%.0f")) {
    atmosphere_.beta_R = rayleigh_coeffs / 10.0e6f;
  }
  float mie_coeff = atmosphere_.beta_M * 10.0e6f;
  if (ImGui::SliderFloat("Mie coeffs", &mie_coeff, 0.0f, 500.0f, "%.0f")) {
    atmosphere_.beta_M = mie_coeff / 10.0e6f;
  }
  ImGui::SliderInt("View dir samples", &atmosphere_.view_dir_samples, 1, 100);
  ImGui::SliderInt("Sun dir samples", &atmosphere_.sun_dir_samples, 1, 100);
  ImGui::End();
}
