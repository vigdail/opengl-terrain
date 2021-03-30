#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <unordered_map>

#include "./scene.h"
#include "./gui/gui_layer.h"

class Application {
 public:
  Application() : Application(1280, 720) {}
  Application(uint32_t width, uint32_t height);
  ~Application();
  void Run();

 private:
  GLFWwindow *window_;
  std::unique_ptr<Scene> scene_;
  uint32_t width_;
  uint32_t height_;

  static void FramebufferSizeCallback(GLFWwindow *window, int width,
                                      int height);
  static void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                          int mode);
  static void MouseButtonCallback(GLFWwindow *window, int button, int action,
                                  int mode);
  static void MouseCallback(GLFWwindow *window, double x, double y);
};
