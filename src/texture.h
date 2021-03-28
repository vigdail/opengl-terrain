#pragma once

#include <glad/glad.h>
#include <stb_image.h>

#include <iostream>

struct TextureSamplerDescriptor {
  uint32_t filter_min = GL_LINEAR;
  uint32_t filter_mag = GL_LINEAR;
  uint32_t wrap_s = GL_REPEAT;
  uint32_t wrap_t = GL_REPEAT;
  uint32_t wrap_r = GL_REPEAT;
};

struct TextureViewDescriptor {
  uint32_t width = 0;
  uint32_t height = 0;
  uint32_t internal_format = GL_RGB;
  uint32_t image_format = GL_RGB;
  uint32_t type = GL_UNSIGNED_BYTE;
};

class TextureBuilder;

class Texture {
  friend TextureBuilder;

 public:
  Texture() noexcept;
  Texture(const Texture &) = delete;
  Texture(Texture &&other) noexcept;
  Texture &operator=(const Texture &) = delete;
  Texture &operator=(Texture &&other);
  ~Texture();
  void Bind() const;
  void Bind(int i) const;
  void BindImage();
  uint32_t GetID() const { return id_; }
  uint32_t GetWidth() const { return view_.width; }
  uint32_t GetHeight() const { return view_.height; }

 private:
  uint32_t id_;
  TextureViewDescriptor view_;

  void Delete();
};

class TextureBuilder {
 public:
  TextureBuilder() noexcept = default;
  TextureBuilder &WithSampler(const TextureSamplerDescriptor &sampler);
  TextureBuilder &WithView(const TextureViewDescriptor &view);
  TextureBuilder &Load(std::string_view path);
  Texture Build() const;

 private:
  TextureSamplerDescriptor sampler_;
  TextureViewDescriptor view_;
  unsigned char *data_;
};
