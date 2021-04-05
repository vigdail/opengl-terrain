#pragma once

#include "buffer.h"

#include <glad/glad.h>
#include <utility>
#include <vector>

enum class PrimitiveTopology {
  POINTS = GL_POINTS,
  LINES = GL_LINES,
  LINE_STRIP = GL_LINE_STRIP,
  TRIANGLE = GL_TRIANGLES,
  TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
  TRIANGLE_FAN = GL_TRIANGLE_FAN,
};

class Mesh {
 public:
  Mesh(PrimitiveTopology topology, size_t count);
  Mesh(Mesh &&other) noexcept;
  Mesh &operator=(Mesh &&other) noexcept;
  Mesh(const Mesh &other) = delete;
  Mesh &operator=(const Mesh &other) = delete;
  ~Mesh();
  void bind() const;
  void unbind() const;
  void addVertexBuffer(VertexBuffer &&buffer);
  void setIndexBuffer(IndexBuffer &&buffer);
  bool isIndexed() const { return index_buffer_.has_value(); }
  void setCount(uint32_t count) { count_ = count; }
  size_t count() const { return count_; }
  PrimitiveTopology getTopology() const;
  void setTopology(PrimitiveTopology topology) { topology_ = topology; }

 private:
  uint32_t id_{};
  size_t count_;
  PrimitiveTopology topology_;
  std::vector<VertexBuffer> vertex_buffers_;
  std::optional<IndexBuffer> index_buffer_;
};
