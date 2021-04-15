#pragma once

#include <vector>

#include "gui_panel.h"

struct GLFWwindow;

class GuiLayer {
 public:
  explicit GuiLayer(GLFWwindow *window);
  ~GuiLayer();
  void update() const;
  void render() const;
  void addPanel(GuiPanel *panel);

 private:
  std::vector<GuiPanel *> panels_;
};
