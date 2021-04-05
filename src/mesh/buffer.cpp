
#include "buffer.h"

#include <utility>

VertexBuffer::VertexBuffer(size_t size) noexcept : layout_{}, size_{size} {
  glCreateBuffers(1, &id_);
  glBindBuffer(GL_ARRAY_BUFFER, id_);
  glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
  glDeleteBuffers(1, &id_);
  id_ = 0;
}

VertexBuffer::VertexBuffer(VertexBuffer &&other) noexcept
    : id_{std::exchange(other.id_, 0)},
      layout_{std::move(other.layout_)},
      size_{std::exchange(other.size_, 0)} {}

VertexBuffer &VertexBuffer::operator=(VertexBuffer &&other) noexcept {
  std::swap(id_, other.id_);
  std::swap(layout_, other.layout_);
  std::swap(size_, other.size_);
  return *this;
}

void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, id_); }
void VertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
BufferLayout VertexBuffer::getLayout() const { return layout_; }

IndexBuffer::IndexBuffer(const std::vector<uint32_t> &indices) noexcept
    : count_{indices.size()} {
  glCreateBuffers(1, &id_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t),
               indices.data(), GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
  glDeleteBuffers(1, &id_);
  id_ = 0;
}

IndexBuffer::IndexBuffer(IndexBuffer &&other) noexcept
    : id_{std::exchange(other.id_, 0)},
      count_{std::exchange(other.count_, 0)} {}

IndexBuffer &IndexBuffer::operator=(IndexBuffer &&other) noexcept {
  std::swap(id_, other.id_);
  std::swap(count_, other.count_);
  return *this;
}

void IndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_); }
void IndexBuffer::unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
size_t IndexBuffer::count() const { return count_; }
