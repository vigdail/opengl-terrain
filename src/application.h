#pragma once

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <memory>
#include <unordered_map>

#include "./gui/gui_layer.h"
#include "./scene.h"
#include "render/renderer.h"

class Application {
 public:
  Application() : Application(1280, 720) {}
  Application(uint32_t width, uint32_t height);
  ~Application();
  void run();

 private:
  GLFWwindow *window_;
  std::unique_ptr<Scene> scene_;
  std::unique_ptr<Renderer> renderer_;
  uint32_t width_;
  uint32_t height_;

  static void framebufferSizeCallback(GLFWwindow *window, int width,
                                      int height);
  static void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                          int mode);
  static void mouseButtonCallback(GLFWwindow *window, int button, int action,
                                  int mode);
  static void mouseCallback(GLFWwindow *window, double x, double y);
};
