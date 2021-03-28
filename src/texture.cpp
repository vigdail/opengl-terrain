#include "texture.h"
#include <memory>
#include <utility>
#include <iostream>

Texture::Texture() noexcept : view_{} { glGenTextures(1, &id_); }

Texture::Texture(Texture &&other) noexcept
    : id_(std::exchange(other.id_, 0)), view_{other.view_} {}

Texture &Texture::operator=(Texture &&other) {
  if (this != &other) {
    Delete();
    std::swap(id_, other.id_);
    std::swap(view_, other.view_);
  }

  return *this;
}

Texture::~Texture() { Delete(); }

void Texture::Delete() {
  glDeleteTextures(1, &id_);
  id_ = 0;
}

void Texture::Bind() const { glBindTexture(GL_TEXTURE_2D, id_); }
void Texture::Bind(int i) const {
  glActiveTexture(GL_TEXTURE0 + i);
  glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture::BindImage() {
  glActiveTexture(GL_TEXTURE0);
  glBindImageTexture(0, id_, 0, GL_FALSE, 0, GL_WRITE_ONLY,
                     view_.internal_format);
}

TextureBuilder &TextureBuilder::WithSampler(
    const TextureSamplerDescriptor &sampler) {
  sampler_ = sampler;
  return *this;
}

TextureBuilder &TextureBuilder::WithView(const TextureViewDescriptor &view) {
  view_ = view;
  return *this;
}

TextureBuilder &TextureBuilder::Load(std::string_view path) {
  if (data_) {
    stbi_image_free(data_);
  }

  int width;
  int height;
  int n_channels;
  data_ = stbi_load(path.data(), &width, &height, &n_channels, 0);

  if (!data_) {
    std::cerr << "ERROR::TEXTURE: Unable to load texture from file: " << path
              << std::endl;
    throw std::runtime_error("Failer to load texture from file");
  }

  view_.width = width;
  view_.height = height;

  if (n_channels == 1) {
    view_.image_format = GL_RED;
    view_.internal_format = GL_RED;
  } else if (n_channels == 4) {
    view_.image_format = GL_RGBA;
    view_.internal_format = GL_RGBA;
  }

  return *this;
}

Texture TextureBuilder::Build() const {
  Texture texture;

  texture.view_ = view_;
  glBindTexture(GL_TEXTURE_2D, texture.id_);
  glTexImage2D(GL_TEXTURE_2D, 0, view_.internal_format, view_.width,
               view_.height, 0, view_.image_format, view_.type, data_);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampler_.filter_min);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampler_.filter_mag);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sampler_.wrap_s);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, sampler_.wrap_t);

  glBindTexture(GL_TEXTURE_2D, 0);

  if (data_) {
    stbi_image_free(data_);
  }

  return texture;
}
