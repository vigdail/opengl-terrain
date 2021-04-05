#pragma once

#include <cstddef>
#include <glad/glad.h>

enum class VertexFormat {
  FLOAT = GL_FLOAT,
};

struct VertexAttribute {
  VertexFormat type = VertexFormat::FLOAT;
  size_t count;
  size_t offset;
  uint32_t location;
  bool normalized;
};
