#pragma once

#include <GLFW/glfw3.h>

class GUILayer {
 public:
  explicit GUILayer(int width, int height);
  void Update(float delta_time);
  void Render();
  // @TODO: Refactor this
  void OnMouseButtonEvent(int button, int action, int mode);
  void OnKeyEvent(int key, int scancode, int action, int mode);
  void OnMousePositionEvent(double x, double y);

 private:
 private:
  int width_;
  int height_;
};
