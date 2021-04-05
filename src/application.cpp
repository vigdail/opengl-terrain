#include "application.h"

#include <iostream>
#include <string>

void debugMessageCallback(unsigned source, unsigned type, unsigned id,
                          unsigned severity, int length, const char *message,
                          const void *user_param) {
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

  int screen_width, scree_height;
  glfwGetMonitorWorkarea(monitor, nullptr, nullptr, &screen_width,
                         &scree_height);
  int xpos = (screen_width - static_cast<int>(width)) / 2;
  int ypos = (scree_height - static_cast<int>(height)) / 2;
  glfwSetWindowPos(window_, xpos, ypos);

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    std::cout << "Failed to initialize GLAD" << std::endl;
  }

  glfwSetWindowUserPointer(window_, this);

  glfwSetKeyCallback(window_, keyCallback);
  glfwSetCursorPosCallback(window_, mouseCallback);
  glfwSetMouseButtonCallback(window_, mouseButtonCallback);
  glfwSetFramebufferSizeCallback(window_, framebufferSizeCallback);

#ifndef NDEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(debugMessageCallback, nullptr);

  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                        GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  glViewport(0, 0, width, height);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  scene_ = std::make_unique<Scene>(width, height);
  renderer_ = std::make_unique<Renderer>();
}

Application::~Application() { glfwTerminate(); }

void Application::run() {
  float last_time = 0.0f;
  while (!glfwWindowShouldClose(window_)) {
    auto current_time = (float)glfwGetTime();
    float delta_time = current_time - last_time;
    last_time = current_time;

    int fps = floor(1.0 / delta_time);
    std::string title = "OpenGL Terrain | " + std::to_string(fps);
    glfwSetWindowTitle(window_, title.c_str());

    glfwPollEvents();

    scene_->processInput(delta_time);

    scene_->update(delta_time);

    // scene_->render();
    renderer_->render(scene_.get());

    glfwSwapBuffers(window_);
  }
}

void Application::keyCallback(GLFWwindow *window, int key, int scancode,
                              int action, int mode) {
  auto self = static_cast<Application *>(glfwGetWindowUserPointer(window));
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  // @TODO: Find out a proper way to handle cursor visibility
  if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
    int cursor_mode;
    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
      cursor_mode = GLFW_CURSOR_NORMAL;
    } else {
      cursor_mode = GLFW_CURSOR_DISABLED;
    }
    glfwSetInputMode(window, GLFW_CURSOR, cursor_mode);
  }

  self->scene_->onKeyEvent(key, scancode, action, mode);
}

void Application::mouseCallback(GLFWwindow *window, double x, double y) {
  auto self = static_cast<Application *>(glfwGetWindowUserPointer(window));
  self->scene_->onMousePositionEvent(x, y);
}

void Application::mouseButtonCallback(GLFWwindow *window, int button,
                                      int action, int mode) {
  auto self = static_cast<Application *>(glfwGetWindowUserPointer(window));
  self->scene_->onMouseButtonEvent(button, action, mode);
}

void Application::framebufferSizeCallback(GLFWwindow *window, int width,
                                          int height) {
  glViewport(0, 0, width, height);
}
