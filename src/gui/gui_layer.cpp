#include "gui_layer.h"

#include <imgui/imgui_impl_opengl3.h>

GUILayer::GUILayer(int width, int height) : width_(width), height_(height) {
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

void GUILayer::Update(float delta_time) {
  ImGuiIO &io = ImGui::GetIO();

  io.DisplaySize = ImVec2(width_, height_);
  io.DeltaTime = delta_time;
}

void GUILayer::Render() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui::NewFrame();

  // @TODO: replace this with actual UI
  static float f = 0.0f;
  static int counter = 0;
  ImGui::Begin("Hello, world!");
  ImGui::Text("This is some useful text.");
  ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
  if (ImGui::Button("Button")) {
    counter++;
  }
  ImGui::SameLine();
  ImGui::Text("counter = %d", counter);
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::End();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUILayer::OnMouseButtonEvent(int button, int action, int mode) {
  bool state;
  if (action == GLFW_PRESS) {
    state = true;
  } else if (action == GLFW_RELEASE) {
    state = false;
  }

  ImGuiIO &io = ImGui::GetIO();
  io.MouseDown[button] = state;
}

void GUILayer::OnKeyEvent(int key, int scancode, int action, int mode) {
  bool state;
  if (action == GLFW_PRESS) {
    state = true;
  } else if (action == GLFW_RELEASE) {
    state = false;
  }

  ImGuiIO &io = ImGui::GetIO();
  io.KeysDown[key] = state;
}

void GUILayer::OnMousePositionEvent(double x, double y) {
  ImGuiIO &io = ImGui::GetIO();
  io.MousePos.x = x;
  io.MousePos.y = y;
}
