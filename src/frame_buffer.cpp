
#include "frame_buffer.h"
#include <utility>

FrameBuffer::FrameBuffer(FrameBuffer::Spec spec) noexcept
    : spec_(std::move(spec)) {
  glGenFramebuffers(1, &id_);

  glBindFramebuffer(GL_FRAMEBUFFER, id_);

  if (!spec_.color_formats.empty()) {
    color_attachments_.reserve(spec_.color_formats.size());
  }

  auto texture_target = [](bool multisampled) -> uint32_t {
    return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
  };

  uint32_t index = 0;
  for (auto format : spec_.color_formats) {
    TextureViewDescriptor view{};
    view.width = spec_.width;
    view.height = spec_.height;
    view.internal_format = format;

    Texture texture = TextureBuilder().withView(view).build();

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index++,
                           texture_target(spec_.is_multisampled),
                           texture.getId(), 0);

    color_attachments_.push_back(std::move(texture));
  }

  if (spec_.depth_format != 0) {
    TextureViewDescriptor view{};
    view.width = spec_.width;
    view.height = spec_.height;
    view.image_format = GL_DEPTH_COMPONENT;
    view.internal_format = spec_.depth_format;
    view.type = GL_FLOAT;

    Texture texture = TextureBuilder().withView(view).build();

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           texture_target(spec_.is_multisampled),
                           texture.getId(), 0);

    depth_attachment_ = std::move(texture);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::FrameBuffer(FrameBuffer &&other) noexcept
    : id_(std::exchange(other.id_, 0)), spec_(std::move(other.spec_)),
      color_attachments_(std::move(other.color_attachments_)),
      depth_attachment_(std::move(other.depth_attachment_)) {}

FrameBuffer &FrameBuffer::operator=(FrameBuffer &&other) noexcept {
  if (this != &other) {
    drop();
    std::swap(id_, other.id_);
    std::swap(spec_, other.spec_);
    std::swap(color_attachments_, other.color_attachments_);
    std::swap(depth_attachment_, other.depth_attachment_);
  }

  return *this;
}

FrameBuffer::~FrameBuffer() { drop(); }

void FrameBuffer::drop() {
  glDeleteFramebuffers(1, &id_);
  id_ = 0;
  color_attachments_.clear();
  depth_attachment_ = {};
}

void FrameBuffer::bind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, id_);
  glViewport(0, 0, spec_.width, spec_.height);
}

void FrameBuffer::unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
