#include "gui_layer.h"

#include <GLFW/glfw3.h>
#include <imgui/imgui_impl_opengl3.h>

GuiLayer::GuiLayer(int width, int height)
    : width_(width), height_(height), panels_() {
  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGuiIO &io = ImGui::GetIO();
  io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
  io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

  io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
  io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
  io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
  io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
  io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
  io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
  io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
  io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
  io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
  io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
  io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
  io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
  io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
  io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
  io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
  io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
  io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
  io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
  io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
  io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
  io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
  io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

  ImGui_ImplOpenGL3_Init("#version 450");
}

GuiLayer::~GuiLayer() {
  for (auto panel : panels_) {
    delete panel;
  }
}

void GuiLayer::addPanel(GuiPanel *panel) { panels_.push_back(panel); }

void GuiLayer::update(float delta_time) const {
  ImGuiIO &io = ImGui::GetIO();

  io.DisplaySize = ImVec2(static_cast<float>(width_), static_cast<float>(height_));
  io.DeltaTime = delta_time;
}

void GuiLayer::render() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui::NewFrame();

  for (auto panel : panels_) {
    panel->render();
  }

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GuiLayer::onMouseButtonEvent(int button, int action, int mode) {
  bool state = false;
  if (action == GLFW_PRESS) {
    state = true;
  } else if (action == GLFW_RELEASE) {
    state = false;
  }

  ImGuiIO &io = ImGui::GetIO();
  io.MouseDown[button] = state;
}

void GuiLayer::onKeyEvent(int key, int scancode, int action, int mode) {
  bool state = false;
  if (action == GLFW_PRESS) {
    state = true;
  } else if (action == GLFW_RELEASE) {
    state = false;
  }

  ImGuiIO &io = ImGui::GetIO();
  io.KeysDown[key] = state;
}

void GuiLayer::onMousePositionEvent(double x, double y) {
  ImGuiIO &io = ImGui::GetIO();
  io.MousePos.x = static_cast<float>(x);
  io.MousePos.y = static_cast<float>(y);
}
