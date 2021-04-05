#pragma once

#include "texture.h"
#include <optional>
#include <vector>

class FrameBuffer {
 public:
  struct Spec {
    uint32_t width;
    uint32_t height;
    bool is_multisampled = false;
    std::vector<uint32_t> color_formats;
    uint32_t depth_format;
  };

  explicit FrameBuffer(Spec spec) noexcept;
  FrameBuffer(const FrameBuffer &) = delete;
  FrameBuffer(FrameBuffer &&) noexcept;
  FrameBuffer &operator=(const FrameBuffer &) = delete;
  FrameBuffer &operator=(FrameBuffer &&) noexcept;
  virtual ~FrameBuffer();

  Texture *getTexture(int i) { return &color_attachments_[i]; }
  Texture *getDepth() { return &depth_attachment_.value(); }

 public:
  void bind() const;
  void unbind() const;

 protected:
  uint32_t id_{};
  Spec spec_;
  std::vector<Texture> color_attachments_;
  std::optional<Texture> depth_attachment_;

 private:
  void drop();
};
