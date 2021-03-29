
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

VertexBuffer::VertexBuffer(VertexBuffer &&other)
    : id_{std::exchange(other.id_, 0)},
      layout_{other.layout_},
      size_{std::exchange(other.size_, 0)} {}

VertexBuffer &VertexBuffer::operator=(VertexBuffer &&other) {
  std::swap(id_, other.id_);
  std::swap(layout_, other.layout_);
  std::swap(size_, other.size_);
  return *this;
}

void VertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, id_); }
void VertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
BufferLayout VertexBuffer::GetLayout() const { return layout_; }

IndexBuffer::IndexBuffer(IndexBuffer &&other)
    : id_{std::exchange(other.id_, 0)},
      count_{std::exchange(other.count_, 0)} {}

IndexBuffer &IndexBuffer::operator=(IndexBuffer &&other) {
  std::swap(id_, other.id_);
  std::swap(count_, other.count_);
  return *this;
}

void IndexBuffer::Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_); }
void IndexBuffer::Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
size_t IndexBuffer::Count() const { return count_; }
