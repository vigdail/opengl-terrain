#pragma once

#include "vertex.h"

#include <vector>
#include <glad/glad.h>
#include <cassert>
#include <optional>

struct BufferLayout {
  size_t stride;
  std::vector<VertexAttribute> attributes;
};

class VertexBuffer {
 public:
  explicit VertexBuffer(size_t size) noexcept : layout_{}, size_{size} {
    glCreateBuffers(1, &id_);
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
  }

  template <typename Vertex>
  VertexBuffer(const std::vector<Vertex> &vertices,
               const BufferLayout &layout) noexcept
      : layout_{layout}, size_{vertices.size() * sizeof(Vertex)} {
    glCreateBuffers(1, &id_);
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 vertices.data(), GL_STATIC_DRAW);
  }

  virtual ~VertexBuffer() {
    glDeleteBuffers(1, &id_);
    id_ = 0;
  }

  VertexBuffer(const VertexBuffer &other) = delete;
  VertexBuffer(VertexBuffer &&other);
  VertexBuffer &operator=(const VertexBuffer &other) = delete;
  VertexBuffer &operator=(VertexBuffer &&other);
  template <typename Vertex>
  void SetData(const std::vector<Vertex> &vertices,
               const BufferLayout &layout) {
    assert(vertices.size * sizeof(Vertex) <= size_);

    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex),
                    vertices.data());
  }
  void Bind() const { glBindBuffer(GL_ARRAY_BUFFER, id_); }
  void Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
  BufferLayout GetLayout() const { return layout_; }

 private:
  uint32_t id_;
  BufferLayout layout_;
  size_t size_;
};

class IndexBuffer {
 public:
  IndexBuffer(const IndexBuffer &other) = delete;
  IndexBuffer(IndexBuffer &&other);
  IndexBuffer &operator=(const IndexBuffer &other) = delete;
  IndexBuffer &operator=(IndexBuffer &&other);
  void Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_); }
  void Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
  size_t Count() const { return count_; }

 private:
  uint32_t id_;
  size_t count_;
};
