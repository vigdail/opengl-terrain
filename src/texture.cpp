#include "texture.h"

Texture::Texture()
    : internal_format(GL_RGB),
      image_format(GL_RGB),
      wrap_s(GL_CLAMP_TO_EDGE),
      wrap_t(GL_CLAMP_TO_EDGE),
      filter_min(GL_LINEAR),
      filter_mag(GL_LINEAR),
      width_(0),
      height_(0) {
  glGenTextures(1, &ID_);
}

void Texture::Generate(unsigned int width, unsigned int height,
                       unsigned char *data) {
  width_ = width;
  height_ = height;

  glBindTexture(GL_TEXTURE_2D, ID_);
  glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width_, height_, 0,
               image_format, GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mag);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind() { glBindTexture(GL_TEXTURE_2D, ID_); }
