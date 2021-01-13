#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>

#include "./game.h"

class Application {
 public:
  Application() : Application(800, 600) {}
  Application(unsigned int width, unsigned int height);
  ~Application();
  void Run();

 private:
  GLFWwindow *window_;
  std::unique_ptr<Game> game_;
  unsigned int width_;
  unsigned int height_;

  static void FramebufferSizeCallback(GLFWwindow *window, int width,
                                      int height);
  static void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                          int mode);
  static void MouseCallback(GLFWwindow *window, double x, double y);
};
