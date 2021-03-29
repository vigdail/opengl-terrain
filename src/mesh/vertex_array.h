#pragma once

#include "buffer.h"

#include <glad/glad.h>
#include <vector>
#include <utility>

class VertexArray {
 public:
  VertexArray();
  VertexArray(VertexArray &&other);
  VertexArray &operator=(VertexArray &&other);
  ~VertexArray();
  void Bind() const;
  void Unbind() const;
  void AddVertexBuffer(VertexBuffer &&buffer);
  void SetIndexBuffer(IndexBuffer &&buffer);

 private:
  uint32_t id_;
  std::vector<VertexBuffer> vertex_buffers_;
  std::optional<IndexBuffer> index_buffer_;
};
