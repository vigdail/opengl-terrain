#include "texture.h"
#include <memory>
#include <utility>
#include <iostream>

Texture::Texture() noexcept
    : internal_format(GL_RGB),
      image_format(GL_RGB),
      wrap_s(GL_CLAMP_TO_EDGE),
      wrap_t(GL_CLAMP_TO_EDGE),
      filter_min(GL_LINEAR),
      filter_mag(GL_LINEAR),
      type(GL_UNSIGNED_BYTE),
      width_(0),
      height_(0) {
  glGenTextures(1, &ID_);
}

Texture::Texture(Texture &&other) noexcept
    : internal_format(other.internal_format),
      image_format(other.image_format),
      wrap_s(other.wrap_s),
      wrap_t(other.wrap_t),
      filter_min(other.filter_min),
      filter_mag(other.filter_mag),
      type(other.type),
      ID_(std::exchange(other.ID_, 0)),
      width_(other.width_),
      height_(other.height_) {}

Texture &Texture::operator=(Texture &&other) {
  if (this != &other) {
    Delete();
    std::swap(internal_format, other.internal_format);
    std::swap(image_format, other.image_format);
    std::swap(wrap_s, other.wrap_s);
    std::swap(wrap_t, other.wrap_t);
    std::swap(filter_min, other.filter_min);
    std::swap(filter_mag, other.filter_mag);
    std::swap(width_, other.width_);
    std::swap(height_, other.height_);
    std::swap(type, other.type);
    std::swap(ID_, other.ID_);
  }

  return *this;
}

Texture::~Texture() { Delete(); }

void Texture::Delete() {
  glDeleteTextures(1, &ID_);
  ID_ = 0;
}

void Texture::Generate(unsigned int width, unsigned int height,
                       unsigned char *data) {
  width_ = width;
  height_ = height;

  glBindTexture(GL_TEXTURE_2D, ID_);
  glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width_, height_, 0,
               image_format, type, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mag);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind() const { glBindTexture(GL_TEXTURE_2D, ID_); }
void Texture::Bind(int i) const {
  glActiveTexture(GL_TEXTURE0 + i);
  glBindTexture(GL_TEXTURE_2D, ID_);
}

void Texture::BindImage() {
  glActiveTexture(GL_TEXTURE0);
  glBindImageTexture(0, ID_, 0, GL_FALSE, 0, GL_WRITE_ONLY, internal_format);
}
