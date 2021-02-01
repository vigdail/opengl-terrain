#pragma once

#include <vector>

#include "gui_panel.h"

class GUILayer {
 public:
  explicit GUILayer(int width, int height);
  ~GUILayer();
  void Update(float delta_time);
  void Render();
  // @TODO: Refactor this
  void OnMouseButtonEvent(int button, int action, int mode);
  void OnKeyEvent(int key, int scancode, int action, int mode);
  void OnMousePositionEvent(double x, double y);
  void AddPanel(GUIPanel *panel);

 private:
  int width_;
  int height_;

  std::vector<GUIPanel *> panels_;
};
