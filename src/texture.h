#pragma once

#include <glad/glad.h>

class Texture {
 public:
  Texture();
  void Bind();
  void Generate(unsigned int width, unsigned int height, unsigned char *data);
  void Delete() { glDeleteTextures(1, &ID_); }

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
};
