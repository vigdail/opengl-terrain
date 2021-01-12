#include "application.h"

#include <iostream>

Application::Application(unsigned int width, unsigned int height)
    : width_(width), height_(height) {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, false);

  window_ = glfwCreateWindow(width, height, "Breakout GL", nullptr, nullptr);
  glfwMakeContextCurrent(window_);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
  }

  glfwSetWindowUserPointer(window_, this);

  glfwSetKeyCallback(window_, KeyCallback);
  glfwSetFramebufferSizeCallback(window_, FramebufferSizeCallback);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE);

  glViewport(0, 0, width, height);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  game_ = std::make_unique<Game>(width, height);
}

Application::~Application() { glfwTerminate(); }

void Application::Run() {
  float last_time = 0.0f;
  while (!glfwWindowShouldClose(window_)) {
    float current_time = glfwGetTime();
    float delta_time = current_time - last_time;
    last_time = current_time;
    glfwPollEvents();

    game_->ProcessInput(delta_time);

    game_->Update(delta_time);

    glClear(GL_COLOR_BUFFER_BIT);
    game_->Render();

    glfwSwapBuffers(window_);
  }
}

void Application::KeyCallback(GLFWwindow *window, int key, int scancode,
                              int action, int mode) {
  auto self = static_cast<Application *>(glfwGetWindowUserPointer(window));
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (action == GLFW_PRESS) {
    self->game_->SetKeyPressed(key);
  } else if (action == GLFW_RELEASE) {
    self->game_->SetKeyReleased(key);
  }
}

void Application::FramebufferSizeCallback(GLFWwindow *window, int width,
                                          int height) {
  glViewport(0, 0, width, height);
}
