#pragma once

class GuiPanel {
 public:
  virtual ~GuiPanel() = default;
  virtual void render() = 0;
};
