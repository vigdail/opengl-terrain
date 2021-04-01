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
  explicit VertexBuffer(size_t size) noexcept;

  template <typename Vertex>
  VertexBuffer(const std::vector<Vertex> &vertices,
               const BufferLayout &layout) noexcept
      : layout_{layout}, size_{vertices.size() * sizeof(Vertex)} {
    glCreateBuffers(1, &id_);
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 vertices.data(), GL_STATIC_DRAW);
  }
  virtual ~VertexBuffer();

  VertexBuffer(const VertexBuffer &other) = delete;
  VertexBuffer(VertexBuffer &&other);
  VertexBuffer &operator=(const VertexBuffer &other) = delete;
  VertexBuffer &operator=(VertexBuffer &&other);

  template <typename Vertex>
  void SetData(const std::vector<Vertex> &vertices,
               const BufferLayout &layout) {
    assert(vertices.size() * sizeof(Vertex) <= size_);

    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex),
                    vertices.data());
  }
  void Bind() const;
  void Unbind() const;
  BufferLayout GetLayout() const;

 private:
  uint32_t id_;
  BufferLayout layout_;
  size_t size_;
};

class IndexBuffer {
 public:
  explicit IndexBuffer(const std::vector<uint32_t> &indices) noexcept;
  IndexBuffer(const IndexBuffer &other) = delete;
  IndexBuffer(IndexBuffer &&other);
  IndexBuffer &operator=(const IndexBuffer &other) = delete;
  IndexBuffer &operator=(IndexBuffer &&other);
  virtual ~IndexBuffer();
  void Bind() const;
  void Unbind() const;
  size_t Count() const;

 private:
  uint32_t id_;
  size_t count_;
};
