#pragma once

class RenderContext {
 public:
  const glm::vec4 &getClipPlane() const {
    return clip_plane_;
  }
  void setClipPlane(const glm::vec4 &clip_plane) {
    clip_plane_ = clip_plane;
  }

 private:
  glm::vec4 clip_plane_{};
};
