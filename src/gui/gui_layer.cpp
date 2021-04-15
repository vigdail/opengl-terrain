#include "gui_layer.h"

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

GuiLayer::GuiLayer(GLFWwindow *window) : panels_() {
  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 450");
}

GuiLayer::~GuiLayer() {
  for (auto panel : panels_) {
    delete panel;
  }
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void GuiLayer::addPanel(GuiPanel *panel) { panels_.push_back(panel); }

void GuiLayer::update() const {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void GuiLayer::render() const {
  ImGui::Begin("Settings");
  ImGui::Text("Stats: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  for (auto panel : panels_) {
    panel->render();
  }
  ImGui::End();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}