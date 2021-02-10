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
    std::vector<unsigned int> color_formats;
    unsigned int depth_format;
  };

  explicit FrameBuffer(Spec spec) noexcept;
  FrameBuffer(const FrameBuffer &) = delete;
  FrameBuffer(FrameBuffer &&) noexcept;
  FrameBuffer &operator=(const FrameBuffer &) = delete;
  FrameBuffer &operator=(FrameBuffer &&);
  virtual ~FrameBuffer();

 public:
  void Bind();
  void Unbind();

 protected:
  unsigned int ID_;
  unsigned int RBO_;
  Spec spec_;
  std::vector<Texture> color_attachments_;
  std::optional<Texture> depth_attachment_;

 private:
  void Delete();
};
