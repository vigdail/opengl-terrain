#pragma once

#include <GLFW/glfw3.h>

class GUILayer {
 public:
  explicit GUILayer(int width, int height);
  // void OnInit();
  // @TODO: Process events
  void OnEvent();
  void Update(float delta_time);
  void Render();

 private:
  int width_;
  int height_;
};
