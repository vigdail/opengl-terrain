#pragma once

#include "buffer.h"

#include <glad/glad.h>
#include <vector>
#include <utility>

enum class PrimitiveTopology {
  Points = GL_POINTS,
  Lines = GL_LINES,
  LineStrip = GL_LINE_STRIP,
  Triangle = GL_TRIANGLES,
  TriangleStrip = GL_TRIANGLE_STRIP,
  TriangleFan = GL_TRIANGLE_FAN,
};

class Mesh {
 public:
  Mesh(PrimitiveTopology topology, size_t count);
  Mesh(Mesh &&other);
  Mesh &operator=(Mesh &&other);
  Mesh(const Mesh &other) = delete;
  Mesh &operator=(const Mesh &other) = delete;
  ~Mesh();
  void Bind() const;
  void Unbind() const;
  void AddVertexBuffer(VertexBuffer &&buffer);
  void SetIndexBuffer(IndexBuffer &&buffer);
  bool IsIndexed() const { return index_buffer_.has_value(); }
  void SetCount(uint32_t count) { count_ = count; }
  size_t Count() const { return count_; }
  PrimitiveTopology GetTopology() const;
  void SetTopology(PrimitiveTopology topology) { topology_ = topology; }

 private:
  uint32_t id_;
  size_t count_;
  PrimitiveTopology topology_;
  std::vector<VertexBuffer> vertex_buffers_;
  std::optional<IndexBuffer> index_buffer_;
};
