#pragma once

#include <vector>

#include "gui_panel.h"

class GuiLayer {
 public:
  explicit GuiLayer(int width, int height);
  ~GuiLayer();
  void update(float delta_time) const;
  void render();
  // @TODO: Refactor this
  static void onMouseButtonEvent(int button, int action, int mode);
  static void onKeyEvent(int key, int scancode, int action, int mode);
  static void onMousePositionEvent(double x, double y);
  void addPanel(GuiPanel *panel);

 private:
  int width_;
  int height_;

  std::vector<GuiPanel *> panels_;
};
