#include "application.h"

#include <iostream>
#include <string>

void DebugMessageCallback(unsigned source, unsigned type, unsigned id,
                          unsigned severity, int length, const char *message,
                          const void *userParam) {
  std::cerr << message << std::endl;
}

Application::Application(uint32_t width, uint32_t height)
    : width_(width), height_(height) {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, false);

  window_ = glfwCreateWindow(width, height, "OpenGL Terrain", nullptr, nullptr);
  glfwMakeContextCurrent(window_);
  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  if (monitor == nullptr) {
    std::cerr << "Failed to get primary monitor" << std::endl;
    glfwTerminate();
  }

  int screenWidth, screeHeight;
  glfwGetMonitorWorkarea(monitor, NULL, NULL, &screenWidth, &screeHeight);
  glfwSetWindowPos(window_, (screenWidth - width) / 2,
                   (screeHeight - height) / 2);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
  }

  glfwSetWindowUserPointer(window_, this);

  glfwSetKeyCallback(window_, KeyCallback);
  glfwSetCursorPosCallback(window_, MouseCallback);
  glfwSetMouseButtonCallback(window_, MouseButtonCallback);
  glfwSetFramebufferSizeCallback(window_, FramebufferSizeCallback);

#ifndef NDEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(DebugMessageCallback, nullptr);

  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                        GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  glViewport(0, 0, width, height);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  scene_ = std::make_unique<Scene>(width, height);
  renderer_ = std::make_unique<Renderer>();
}

Application::~Application() { glfwTerminate(); }

void Application::Run() {
  float last_time = 0.0f;
  while (!glfwWindowShouldClose(window_)) {
    float current_time = glfwGetTime();
    float delta_time = current_time - last_time;
    last_time = current_time;

    int fps = floor(1.0f / delta_time);
    std::string title = "OpenGL Terrain | " + std::to_string(fps);
    glfwSetWindowTitle(window_, title.c_str());

    glfwPollEvents();

    scene_->ProcessInput(delta_time);

    scene_->Update(delta_time);

    // scene_->Render();
    renderer_->Render(scene_.get());

    glfwSwapBuffers(window_);
  }
}

void Application::KeyCallback(GLFWwindow *window, int key, int scancode,
                              int action, int mode) {
  auto self = static_cast<Application *>(glfwGetWindowUserPointer(window));
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  // @TODO: Find out a proper way to handle cursor visibility
  if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
    int cursore_mode;
    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
      cursore_mode = GLFW_CURSOR_NORMAL;
    } else {
      cursore_mode = GLFW_CURSOR_DISABLED;
    }
    glfwSetInputMode(window, GLFW_CURSOR, cursore_mode);
  }

  self->scene_->OnKeyEvent(key, scancode, action, mode);
}

void Application::MouseCallback(GLFWwindow *window, double x, double y) {
  auto self = static_cast<Application *>(glfwGetWindowUserPointer(window));
  self->scene_->OnMousePositionEvent(x, y);
}

void Application::MouseButtonCallback(GLFWwindow *window, int button,
                                      int action, int mode) {
  auto self = static_cast<Application *>(glfwGetWindowUserPointer(window));
  self->scene_->OnMouseButtonEvent(button, action, mode);
}

void Application::FramebufferSizeCallback(GLFWwindow *window, int width,
                                          int height) {
  glViewport(0, 0, width, height);
}
