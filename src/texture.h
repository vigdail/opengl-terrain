#pragma once

#include <glad/glad.h>

class Texture {
 public:
  Texture();
  Texture(const Texture &) = delete;
  Texture(Texture &&other);
  Texture &operator=(const Texture &) = delete;
  Texture &operator=(Texture &&other);
  ~Texture();
  void Bind();
  void Generate(unsigned int width, unsigned int height, unsigned char *data);

  unsigned int internal_format;
  unsigned int image_format;
  unsigned int wrap_s;
  unsigned int wrap_t;
  unsigned int filter_min;
  unsigned int filter_mag;

 private:
  unsigned int ID_;
  unsigned int width_;
  unsigned int height_;

  void Delete();
};
