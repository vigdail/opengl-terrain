
#include "frame_buffer.h"
#include <utility>

FrameBuffer::FrameBuffer(const FrameBuffer::Spec& spec) noexcept : spec_(spec) {
  glGenFramebuffers(1, &ID_);

  glBindFramebuffer(GL_FRAMEBUFFER, ID_);

  if (spec_.color_formats.size() > 0) {
    color_attachments_.reserve(spec_.color_formats.size());
  }

  auto TextureTarget = [](bool multisampled) {
    return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
  };

  int index = 0;
  for (auto format : spec_.color_formats) {
    TextureViewDescriptor view{};
    view.width = spec_.width;
    view.height = spec_.height;
    view.internal_format = format;

    Texture texture = TextureBuilder().WithView(view).Build();

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index++,
                           TextureTarget(spec_.is_multisampled),
                           texture.GetID(), 0);

    color_attachments_.push_back(std::move(texture));
  }

  if (spec_.depth_format != 0) {
    TextureViewDescriptor view{};
    view.width = spec_.width;
    view.height = spec_.height;
    view.image_format = GL_DEPTH_COMPONENT;
    view.internal_format = spec_.depth_format;
    view.type = GL_FLOAT;

    Texture texture = TextureBuilder().WithView(view).Build();

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           TextureTarget(spec_.is_multisampled),
                           texture.GetID(), 0);

    depth_attachment_ = std::move(texture);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept
    : ID_(std::exchange(other.ID_, 0)),
      spec_(other.spec_),
      color_attachments_(std::move(other.color_attachments_)),
      depth_attachment_(std::move(other.depth_attachment_)) {}

FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) {
  if (this != &other) {
    Delete();
    std::swap(ID_, other.ID_);
    std::swap(spec_, other.spec_);
    std::swap(color_attachments_, other.color_attachments_);
    std::swap(depth_attachment_, other.depth_attachment_);
  }

  return *this;
}

FrameBuffer::~FrameBuffer() { Delete(); }

void FrameBuffer::Delete() {
  glDeleteFramebuffers(1, &ID_);
  ID_ = 0;
  color_attachments_.clear();
  depth_attachment_ = {};
}

void FrameBuffer::Bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, ID_);
  glViewport(0, 0, spec_.width, spec_.height);
}

void FrameBuffer::Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
