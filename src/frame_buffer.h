#pragma once

#include <vector>
#include <optional>
#include "texture.h"

class FrameBuffer {
 public:
  struct Spec {
    int width;
    int height;
    bool is_multisampled = false;
    std::vector<uint32_t> color_formats;
    uint32_t depth_format;
  };

  explicit FrameBuffer(const Spec &spec) noexcept;
  FrameBuffer(const FrameBuffer &) = delete;
  FrameBuffer(FrameBuffer &&) noexcept;
  FrameBuffer &operator=(const FrameBuffer &) = delete;
  FrameBuffer &operator=(FrameBuffer &&);
  virtual ~FrameBuffer();

  Texture *GetTexture(int i) { return &color_attachments_[i]; }
  Texture *GetDepth() { return &depth_attachment_.value(); }

 public:
  void Bind();
  void Unbind();

 protected:
  uint32_t ID_;
  Spec spec_;
  std::vector<Texture> color_attachments_;
  std::optional<Texture> depth_attachment_;

 private:
  void Delete();
};
