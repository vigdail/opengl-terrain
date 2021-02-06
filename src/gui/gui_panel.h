#pragma once

class GUIPanel {
 public:
  virtual ~GUIPanel() = default;
  virtual void Render() = 0;
};
